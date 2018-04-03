#include "ChessGame.h"

#pragma region Game Functions

ChessPiece* gameGetPieceAt(ChessGame* game, Position pos)
{
	if (posIsInBoard(pos) == false)
		return NULL;

	return game->gameBoard[pos.x][pos.y];
}

void gameSetPieceAt(ChessGame* game, Position pos, ChessPiece* newPiece)
{
	if (posIsInBoard(pos) == false)
		return;

	game->gameBoard[pos.x][pos.y] = newPiece;
}

void gameMovePiece(ChessGame* game, Move move)
{
	ChessPiece* fromPiece = gameGetPieceAt(game, move.from);
	ChessPiece* toPiece = gameGetPieceAt(game, move.to);

	if (toPiece != NULL)
		free(toPiece);

	fromPiece->position = move.to;
	gameSetPieceAt(game, move.to, fromPiece);
	gameSetPieceAt(game, move.from, NULL);
}

void gameMakeMove(ChessGame* game, Move move)
{
	gameMovePiece(game, move);
	logicUpdateGameStatus(game);

	if (game->status == STATUS_NORMAL || game->status == STATUS_CHECK)
		game->currentPlayer = getOpositeColor(game->currentPlayer);
}

ChessGame* gameCopy(ChessGame* game)
{
	ChessGame* copy = (ChessGame*)malloc(sizeof(ChessGame));

	if (copy == NULL)
	{
		mallocError = true;
		return NULL;
	}

	copy->currentPlayer = game->currentPlayer;
	copy->status = game->status;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			ChessPiece* piece = game->gameBoard[i][j];

			if (piece == NULL)
			{
				copy->gameBoard[i][j] = NULL;
			}
			else
			{
				copy->gameBoard[i][j] = (ChessPiece*)malloc(sizeof(ChessPiece));

				if (copy->gameBoard[i][j] == NULL) 
				{
					mallocError = true;
					return NULL;
				}

				copy->gameBoard[i][j]->color = piece->color;
				copy->gameBoard[i][j]->position = piece->position;
				copy->gameBoard[i][j]->type = piece->type;
			}
		}
	}

	return copy;
}

void gameDestroy(ChessGame* game)
{
	if (game == NULL)
		return;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (game->gameBoard[i][j] != NULL)
				free(game->gameBoard[i][j]);
		}
	}

	free(game);
}

MoveOptionsList* gameGetPieceValidMoves(ChessGame* game, Position pos, bool calcThreatened)
{
	MoveOptionsList* validMoves = arrayListCreate(MAX_PIECE_MOVES);
	ChessPiece* piece = gameGetPieceAt(game, pos);

	if (validMoves == NULL)
		return NULL;

	if (piece == NULL)
		return validMoves;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position p = newPos(j, i); // order matters

			if (logicIsValidMove(game, newMove(pos, p)) == IVMR_VALID)
			{
				moveOption* moveOpt = (moveOption*)malloc(sizeof(moveOption));
				moveOpt->move = newMove(pos, p);

				if (calcThreatened)
				{
					moveOpt->isCapturing = (gameGetPieceAt(game, p) != NULL); // move to a pos of other pieace is always capturing (must be other color if valid)
					moveOpt->isThreatened = logicCheckThreatened(game, p, piece->color);
				}

				arrayListAddLast(validMoves, moveOpt);
			}
		}
	}

	return validMoves;
}

MoveOptionsList* gameGetAllValidMoves(ChessGame* game, PLAYER_COLOR color)
{
	MoveOptionsList* allValidMoves = arrayListCreate(MAX_PLAYER_MOVES);

	if (allValidMoves == NULL)
		return NULL;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position p = newPos(j, i); // order matters
			ChessPiece* piece = gameGetPieceAt(game, p);

			if (piece != NULL)
			{
				MoveOptionsList* pieceValidMoves = gameGetPieceValidMoves(game, p, false);
				if (pieceValidMoves == NULL)
				{
					return NULL;
				}

				arrayListAddList(allValidMoves, pieceValidMoves);

				free(pieceValidMoves->elements);
				free(pieceValidMoves);
			}
		}
	}

	return allValidMoves;
}

PLAYER_COLOR getOpositeColor(PLAYER_COLOR color)
{
	return (color == WHITE ? BLACK : WHITE);
}

#pragma endregion

#pragma region Piece Valid Functions (not in header)

bool checkClearPath(ChessGame* game, Move move)
{
	Vector2 step = vecNormilized(vecDiff(move.to, move.from));

	// check that there is no piece between move.from and move.to
	// start at move.from, go one step at a time until move.to
	for (Position p = vecAdd(move.from, step); posEqual(p, move.to) == false; p = vecAdd(p, step))
	{
		if (gameGetPieceAt(game, p) != NULL)
			return false;
	}

	return true;
}

bool isValidMovePawn(ChessGame* game, Move move)
{
	ChessPiece* pawn = gameGetPieceAt(game, move.from);
	ChessPiece* toPiece = gameGetPieceAt(game, move.to);

	Vector2 diff = vecDiff(move.from, move.to);
	Vector2 absDiff = vecAbs(diff);
	Vector2 normDiff = vecNormilized(diff);

	// check if moving the right direction
	int colorDirection = (pawn->color == WHITE ? -1 : 1);
	if (normDiff.x != colorDirection)
		return false;

	if (toPiece == NULL) // normal move
	{
		if (diff.y != 0)
			return false;

		if (absDiff.x == 1)
		{
			return true;
		}
		else if (absDiff.x == 2) // if jumping
		{
			bool isJumpValid = ((pawn->color == WHITE) ? (move.from.x == 1) : (move.from.x == 6));
			if (isJumpValid == false)
				return false;

			// check that there is no piece in the middle of the jump
			if (gameGetPieceAt(game, vecAdd(move.from, normDiff)) != NULL)
				return false;
		}
		else if (absDiff.x > 2)
		{
			return false;
		}
	}
	else if (toPiece != NULL) // capturing move
	{
		if (toPiece->color == pawn->color)
			return false;

		if (posEqual(absDiff, newPos(1, 1)) == false)
			return false;
	}

	return true;
}

bool isValidMoveKnight(ChessGame* game, Move move)
{
	Vector2 absDiff = vecAbs(vecDiff(move.from, move.to));

	return (absDiff.x == 2 && absDiff.y == 1) ||
		(absDiff.x == 1 && absDiff.y == 2);
}

bool isValidMoveBishop(ChessGame* game, Move move)
{
	Vector2 diffAbs = vecAbs(vecDiff(move.from, move.to));

	// check if moving in a valid diagonal
	if (diffAbs.x != diffAbs.y)
		return false;

	return checkClearPath(game, move);
}

bool isValidMoveRook(ChessGame* game, Move move)
{
	Vector2 diff = vecDiff(move.from, move.to);

	// check if moving in only one axis (the case both are zero is detected before)
	if (diff.x != 0 && diff.y != 0)
		return false;

	return checkClearPath(game, move);
}

bool isValidMoveQueen(ChessGame* game, Move move)
{
	return isValidMoveBishop(game, move) || isValidMoveRook(game, move);
}

bool isValidMoveKing(ChessGame* game, Move move)
{
	Vector2 absDiff = vecAbs(vecDiff(move.from, move.to));
	return absDiff.x <= 1 && absDiff.y <= 1;
}

typedef bool(*ValidFuncPtr)(ChessGame*, Move);
ValidFuncPtr getValidFuncPtr(PIECE_TYPE type)
{
	switch (type)
	{
	case PAWN:
		return isValidMovePawn;
	case BISHOP:
		return isValidMoveBishop;
	case ROOK:
		return isValidMoveRook;
	case KNIGHT:
		return isValidMoveKnight;
	case QUEEN:
		return isValidMoveQueen;
	case KING:
		return isValidMoveKing;
	}

	return NULL;
}

#pragma endregion

#pragma region Chess Logic

bool logicCheckThreatened(ChessGame* game, Position pos, PLAYER_COLOR currColor)
{
	// copy game to overide piece at pos, without changing the game
	// TODO: check if it damages running time significantly
	ChessGame* copy = gameCopy(game);
	ChessPiece dummyPieace;
	dummyPieace.color = currColor;

	// replace piece at pos to a dummy piece with currColor.
	// neccessary for logicIsValidMove to be correct (the color must be currColor, and also for pawns)
	gameSetPieceAt(copy, pos, &dummyPieace);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position p = newPos(i, j);

			if (gameGetPieceAt(copy, p) != NULL && gameGetPieceAt(copy, p)->color != currColor) // if there is an oponent's pieace at p 
			{
				if (logicIsValidMoveBasic(copy, newMove(p, pos)) == IVMR_VALID) // if the oponent's pieace can eat dummyPiece
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool logicIsKingThreatened(ChessGame* game, PLAYER_COLOR color)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position pos = newPos(i, j);
			ChessPiece* piece = gameGetPieceAt(game, pos);
			if (piece != NULL && piece->type == KING)
				return logicCheckThreatened(game, pos, color);
		}
	}

	return false; // not suppose to get here
}

void logicUpdateGameStatus(ChessGame* game)
{
	bool isKingThreatened = logicIsKingThreatened(game, getOpositeColor(game->currentPlayer));
	MoveOptionsList* moves = gameGetAllValidMoves(game, getOpositeColor(game->currentPlayer));
	bool noMoves = arrayListIsEmpty(moves);

	if (isKingThreatened && noMoves == false)
	{
		game->status = STATUS_CHECK;
	}
	else if (isKingThreatened && noMoves)
	{
		game->status = STATUS_MATE;
	}
	else if (isKingThreatened == false && noMoves)
	{
		game->status = STATUS_DRAW;
	}
	else
	{
		game->status = STATUS_NORMAL;
	}

	arrayListDestroy(moves);
}

IS_VALID_MOVE_RESULT logicIsValidMoveBasic(ChessGame* game, Move move)
{
	ChessPiece* fromPiece = gameGetPieceAt(game, move.from);
	ChessPiece* toPiece = gameGetPieceAt(game, move.to);

	if (posIsInBoard(move.from) == false || posIsInBoard(move.to) == false)
		return IVMR_INVALID_POSITION;

	if (fromPiece == NULL || (fromPiece->color != game->currentPlayer))
		return IVMR_NO_PIECE_IN_POS;

	if (toPiece != NULL && fromPiece->color == toPiece->color)
		return IVMR_ILLEGAL_MOVE;

	if (posEqual(move.from, move.to))
		return IVMR_ILLEGAL_MOVE;

	if (getValidFuncPtr(fromPiece->type)(game, move) == false)
		return IVMR_ILLEGAL_MOVE;

	return IVMR_VALID;
}

IS_VALID_MOVE_RESULT logicIsValidMove(ChessGame* game, Move move)
{
	IS_VALID_MOVE_RESULT basicResult = logicIsValidMoveBasic(game, move);
	if (basicResult != IVMR_VALID)
	{
		return basicResult;
	}
	else
	{
		bool isKingThreatenedAfter;
		ChessGame* copy = gameCopy(game);
		gameMovePiece(copy, move);
		isKingThreatenedAfter = logicIsKingThreatened(copy, game->currentPlayer);
		if (isKingThreatenedAfter)
		{
			bool isKingThreatenedBefore = logicIsKingThreatened(game, game->currentPlayer);
			return (isKingThreatenedBefore ? IVMR_KING_STILL_THREATENED : IVMR_KING_GET_THREATENED);
		}
	}

	return IVMR_VALID;
}

#pragma endregion