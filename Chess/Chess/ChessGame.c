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

	game->currentPlayer = getOpositeColor(game->currentPlayer);
	logicUpdateGameStatus(game);
}

ChessGame* gameCreate()
{
	ChessGame* game = (ChessGame*)malloc(sizeof(ChessGame));

	if (game == NULL)
	{
		mallocError = true;
		return NULL;
	}

	game->status = STATUS_NORMAL;
	game->currentPlayer = WHITE;

	// create board
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			game->gameBoard[j][i] = NULL;
			if (j <= 1 || j >= 6)
			{
				Position pos = newPos(j, i);
				ChessPiece* piece = (ChessPiece*)malloc(sizeof(ChessPiece));
				if (piece == NULL)
				{
					mallocError = true;
					return NULL;
				}

				piece->position = pos;
				gameSetPieceAt(game, pos, piece);

				if (j == 0 || j == 1)
					piece->color = WHITE;
				else
					piece->color = BLACK;

				if (j == 1 || j == 6)
				{
					piece->type = PAWN;
				}
				else
				{
					if (i == 0 || i == 7)
						piece->type = ROOK;
					else if (i == 1 || i == 6)
						piece->type = KNIGHT;
					else if (i == 2 || i == 5)
						piece->type = BISHOP;
					else if (i == 3)
						piece->type = QUEEN;
					else if (i == 4)
						piece->type = KING;
				}
			}
		}
	}
	return game;
}

ChessPiece* chessPieceCopy(ChessPiece* piece)
{
	if (piece == NULL)
	{
		return NULL;
	}
	else
	{
		ChessPiece* copy = (ChessPiece*)malloc(sizeof(ChessPiece));

		if (copy == NULL)
		{
			mallocError = true;
			return NULL;
		}

		copy->color = piece->color;
		copy->position = piece->position;
		copy->type = piece->type;

		return copy;
	}
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
			copy->gameBoard[i][j] = chessPieceCopy(game->gameBoard[i][j]);
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

MoveOptionsList* gameGetPieceValidMoves(ChessGame* game, Position pos, bool calcThreatened, bool returnAfterFirst)
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

				if (moveOpt == NULL)
				{
					mallocError;
					return NULL;
				}

				moveOpt->move = newMove(pos, p);

				if (calcThreatened)
				{
					moveOpt->isCapturing = (gameGetPieceAt(game, p) != NULL); // move to a pos of other pieace is always capturing (must be other color if valid)
					moveOpt->isThreatened = logicCheckThreatened(game, p, piece->color);
				}

				arrayListAddLast(validMoves, moveOpt);

				if (returnAfterFirst)
					return validMoves;
			}
		}
	}

	return validMoves;
}

MoveOptionsList* gameGetAllValidMoves(ChessGame* game, PLAYER_COLOR color, bool returnAfterFirst)
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

			if (piece != NULL && piece->color == color)
			{
				MoveOptionsList* pieceValidMoves = gameGetPieceValidMoves(game, p, false, returnAfterFirst);
				if (pieceValidMoves == NULL)
				{
					return NULL;
				}

				if (pieceValidMoves->actualSize > 0)
				{
					arrayListAddList(allValidMoves, pieceValidMoves);
				}

				free(pieceValidMoves->elements);
				free(pieceValidMoves);

				if (returnAfterFirst && allValidMoves->actualSize > 0)
					return allValidMoves;
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

// check that there is no piece between move.from and move.to
bool checkClearPath(ChessGame* game, Move move)
{
	Vector2 step = vecNormilized(vecDiff(move.to, move.from));

	// start at move.from, go one step at a time until move.to
	for (Position p = vecAdd(move.from, step); !posEqual(p, move.to); p = vecAdd(p, step))
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

	Vector2 diff = vecDiff(move.to, move.from);
	Vector2 absDiff = vecAbs(diff);
	Vector2 normDiff = vecNormilized(diff);

	// check if moving the right direction
	int colorDirection = (pawn->color == WHITE ? 1 : -1);
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
	Vector2 absDiff = vecAbs(vecDiff(move.to, move.from));

	return (absDiff.x == 2 && absDiff.y == 1) ||
		(absDiff.x == 1 && absDiff.y == 2);
}

bool isValidMoveBishop(ChessGame* game, Move move)
{
	Vector2 diffAbs = vecAbs(vecDiff(move.to, move.from));

	// check if moving in a valid diagonal
	if (diffAbs.x != diffAbs.y)
		return false;

	return checkClearPath(game, move);
}

bool isValidMoveRook(ChessGame* game, Move move)
{
	Vector2 diff = vecDiff(move.to, move.from);

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
	Vector2 absDiff = vecAbs(vecDiff(move.to, move.from));
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
	bool result = false;
	// copy game to overide piece at pos, without changing the game
	// TODO: check if it damages running time significantly
	//ChessGame* copy = gameCopy(game);

	ChessPiece* posPiece = gameGetPieceAt(game, pos);
	bool returnToNull = (posPiece == NULL);
	bool changeColor = (posPiece != NULL && posPiece->color != currColor);
	PLAYER_COLOR origColor = game->currentPlayer;

	// replace piece at pos to a piece with currColor.
	// neccessary for logicIsValidMove to be correct (the color must be currColor, and also for pawns)
	if (changeColor)
	{
		posPiece->color = currColor;
	}
	else if (returnToNull)
	{
		ChessPiece* dummyPieace = (ChessPiece*)malloc(sizeof(ChessPiece));
		dummyPieace->color = currColor;
		gameSetPieceAt(game, pos, dummyPieace);
	}

	// logicIsValidMoveBasic checks currentPlayer
	game->currentPlayer = getOpositeColor(currColor);

	for (int i = 0; i < 8 && !result; i++)
	{
		for (int j = 0; j < 8 && !result; j++)
		{
			Position p = newPos(i, j);

			if (gameGetPieceAt(game, p) != NULL && gameGetPieceAt(game, p)->color != currColor) // if there is an oponent's pieace at p 
			{
				if (logicIsValidMoveBasic(game, newMove(p, pos)) == IVMR_VALID) // if the oponent's pieace can eat dummyPiece
				{
					result = true;
				}
			}
		}
	}

	// restore to original game
	game->currentPlayer = origColor;

	if (changeColor)
	{
		posPiece->color = getOpositeColor(currColor);
	}
	else if (returnToNull)
	{
		free(gameGetPieceAt(game, pos));
		gameSetPieceAt(game, pos, NULL);
	}

	return result;
}

bool logicIsKingThreatened(ChessGame* game, PLAYER_COLOR color)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position pos = newPos(i, j);
			ChessPiece* piece = gameGetPieceAt(game, pos);
			if (piece != NULL && piece->type == KING && piece->color == color)
				return logicCheckThreatened(game, pos, color);
		}
	}

	return false; // not suppose to get here
}

void logicUpdateGameStatus(ChessGame* game)
{
	bool isKingThreatened = logicIsKingThreatened(game, game->currentPlayer);
	MoveOptionsList* moves = gameGetAllValidMoves(game, game->currentPlayer, true);
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
		gameDestroy(copy);
		if (isKingThreatenedAfter)
		{
			bool isKingThreatenedBefore = logicIsKingThreatened(game, game->currentPlayer);
			return (isKingThreatenedBefore ? IVMR_KING_STILL_THREATENED : IVMR_KING_GET_THREATENED);
		}
	}

	return IVMR_VALID;
}

#pragma endregion