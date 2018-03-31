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

ChessGame* gameCopy(ChessGame* game)
{
	ChessGame* copy = (ChessGame*)malloc(sizeof(ChessGame));

	if (copy == NULL)
		return NULL;

	copy->currentPlayer = game->currentPlayer;
	copy->status = game->status;
	copy->whiteKing = game->whiteKing;
	copy->blackKing = game->blackKing;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			copy->gameBoard[i][j] = game->gameBoard[i][j];
		}
	}

	return copy;
}

MoveOptionsList* gameGetValidMoves(ChessGame* game, Position pos)
{
	MoveOptionsList* validMoves = arrayListCreate(8 * 4 - 3); // 8*4-3 is the maximum possible moves for a queen
	ChessPiece* piece = gameGetPieceAt(game, pos);

	if (piece == NULL)
		return validMoves;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position p = newPos(i, j);

			if (logicIsValidMove(game, newMove(pos, p)) == IVMR_VALID)
			{
				moveOption* moveOpt = (moveOption*)malloc(sizeof(moveOption));
				moveOpt->pos = p;
				moveOpt->isCapturing = (gameGetPieceAt(game, p) != NULL); // move to a pos of other pieace is always capturing (must be other color if valid)
				moveOpt->isThreatened = logicCheckThreatened(game, p, piece->color);
				arrayListAddLast(validMoves, moveOpt);
			}
		}
	}

	return validMoves;
}

#pragma endregion

#pragma region Piece Valid Functions (not in header)

bool checkClearPath(ChessGame* game, Move move)
{
	Position step = posNormilized(posDiff(move.from, move.to));

	// check that there is no piece between move.from and move.to
	// start at move.from, go one step at a time until move.to
	for (Position p = posAdd(move.from, step); posEqual(p, move.to) == false; p = posAdd(p, step))
	{
		if (gameGetPieceAt(game, p) != NULL)
			return false;
	}

	return true;
}

bool isValidMovePawn(ChessGame* game, Move move)
{
	return true;
}

bool isValidMoveKnight(ChessGame* game, Move move)
{
	Position absDiff = posAbs(posDiff(move.from, move.to));

	return (absDiff.x == 2 && absDiff.y == 1) ||
		(absDiff.x == 1 && absDiff.y == 2);
}

bool isValidMoveBishop(ChessGame* game, Move move)
{
	Position diffAbs = posAbs(posDiff(move.from, move.to));

	// check if moving in a valid diagonal
	if (diffAbs.x != diffAbs.y)
		return false;

	return checkClearPath(game, move);
}

bool isValidMoveRook(ChessGame* game, Move move)
{
	Position diff = posDiff(move.from, move.to);

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
	Position absDiff = posAbs(posDiff(move.from, move.to));
	return absDiff.x <= 1 && absDiff.x <= 1;
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

#pragma region Logic

bool logicCheckThreatened(ChessGame* game, Position pos, PLAYER_COLOR currColor)
{
	ChessGame* copy = gameCopy(game); // copy game to overide piece at pos, without changing the game
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
				if (logicIsValidMove(copy, newMove(p, pos)) == IVMR_VALID) // if the oponent's pieace can eat dummyPiece
				{
					return true;
				}
			}
		}
	}

	return false;
}

void logicUpdateGameStatus(ChessGame* game)
{
}

IS_VALID_MOVE_RESULT logicIsValidMove(ChessGame* game, Move move)
{
	ChessPiece* fromPiece = gameGetPieceAt(game, move.from);
	ChessPiece* toPiece = gameGetPieceAt(game, move.to);

	if (posIsInBoard(move.from) == false || posIsInBoard(move.to) == false)
		return IVMR_INVALID_POSITION;

	if (fromPiece == NULL)
		return IVMR_NO_PIECE_IN_POS;

	if (toPiece != NULL && fromPiece->color == toPiece->color)
		return IVMR_ILLEGAL_MOVE;

	if (posEqual(move.from, move.to))
		return IVMR_ILLEGAL_MOVE;

	if (getValidFuncPtr(fromPiece->type)(game, move) == false)
		return IVMR_ILLEGAL_MOVE;

	ChessPiece* king = ((game->currentPlayer == WHITE) ? game->whiteKing : game->blackKing);
	logicCheckThreatened(game, king->position, game->currentPlayer);
	//return IVMR_KING_STILL_THREATENED;
	//return IVMR_KING_GET_THREATENED;

	return IVMR_VALID;
}

#pragma endregion