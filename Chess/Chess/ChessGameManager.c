#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ChessGameManager.h"
#include "MoveOptionsList.h"
#include "HistoryCircularArray.h"

#pragma region Game Functions

ChessGameManager* createGameManager(int historySize)
{
	if (historySize <= 0) //TODO: check if needed
		return NULL;

	ChessGameManager* manager = (ChessGameManager*)malloc(sizeof(ChessGameManager));

	if (manager == NULL)
		return NULL;

	manager->game->status = STATUS_NORMAL;
	manager->game->currentPlayer = WHITE;
	manager->mode = ONE_PLAYER;
	manager->difficulty = EASY;
	manager->history = circularArrayCreate(historySize);

	if (manager->history == NULL)
	{
		free(manager);
		return NULL;
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			manager->game->gameBoard[i][j] = NULL;
			if (j <= 1 || j >= 6)
			{
				Position pos = newPos(i, j);
				ChessPiece* piece = (ChessPiece*)malloc(sizeof(ChessPiece));
				if (piece == NULL)
					return NULL;

				piece->position = pos;
				gameSetPieceAt(manager, pos, piece);

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
					else if (i == 4)
						piece->type = QUEEN;
					else if (i == 3)
					{
						piece->type = KING;

						if (piece->color == WHITE)
							manager->game->whiteKing = piece;
						else
							manager->game->blackKing = piece;
					}
				}
			}
		}
	}

	return manager;
}

ChessGameManager* gameCopy(ChessGameManager* src)
{
	ChessGameManager* copy = (ChessGameManager*)malloc(sizeof(ChessGameManager));

	if (copy == NULL)
		return NULL;

	copy->game->currentPlayer = src->game->currentPlayer;
	copy->history = NULL; // copy is only for minimax - dont need to copy history
	copy->game->status = src->game->status;
	copy->game->whiteKing = src->game->whiteKing;
	copy->game->blackKing = src->game->blackKing;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			copy->game->gameBoard[i][j] = src->game->gameBoard[i][j];
		}
	}

	return copy;
}

void gameDestroy(ChessGameManager* src)
{
	if (src == NULL)
		return;

	circularArrayDestroy(src->history);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			free(src->game->gameBoard[i][j]);
		}
	}

	free(src);
}

MoveOptionsList* gameGetValidMoves(ChessGameManager* src, Position pos)
{
	MoveOptionsList* validMoves = arrayListCreate(8 * 4 - 3); // 8*4-3 is the maximum possible moves for a queen
	ChessPiece* piece = gameGetPieceAt(src, pos);

	if (piece == NULL)
		return validMoves;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position p = newPos(i, j);

			if (logicIsValidMove(src, newMove(pos, p)) == IVMR_VALID)
			{
				moveOption* moveOpt = (moveOption*)malloc(sizeof(moveOption));
				moveOpt->pos = p;
				moveOpt->isCapturing = (gameGetPieceAt(src, p) != NULL); // move to a pos of other pieace is always capturing (must be other color if valid)
				moveOpt->isThreatened = logicCheckThreatened(src, p, piece->color);
				arrayListAddLast(validMoves, moveOpt);
			}
		}
	}

	return validMoves;
}

bool gameMakeMove(ChessGameManager* src, Move move)
{
	if (logicIsValidMove(src, move) == false)
		return false;

	circularArrayAdd(src->history, gameCopy(src)->game);

	gameMovePiece(src, move);
	logicUpdateGameStatus(src);

	if (src->game->status == STATUS_NORMAL)
		src->game->currentPlayer = (src->game->currentPlayer == WHITE) ? BLACK : WHITE;

	return true;
}

CHESS_GAME_MESSAGE gameUndoPrevMove(ChessGameManager* src)
{
	ChessGame* copy = circularArrayGetCurrent(src->history);
	if (src->history->actualSize == 0)
		return CHESS_GAME_NO_HISTORY;

	src->game->currentPlayer = copy->currentPlayer;
	src->game->whiteKing = copy->whiteKing;
	src->game->blackKing = copy->blackKing;
	src->game->status = copy->status;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			src->game->gameBoard[i][j] = copy->gameBoard[i][j];
		}
	}

	circularArrayRemove(src->history);

	return CHESS_GAME_SUCCESS;
}

void printPiece(ChessPiece* piece)
{
	char c;

	if (piece == NULL)
	{
		printf("_ ");
		return;
	}

	switch (piece->type)
	{
	case PAWN:
		c = 'P';
		break;
	case KNIGHT:
		c = 'N';
		break;
	case ROOK:
		c = 'R';
		break;
	case BISHOP:
		c = 'B';
		break;
	case QUEEN:
		c = 'Q';
		break;
	case KING:
		c = 'K';
		break;
	}

	if (piece->color == WHITE)
		c += 'a' - 'A';

	printf("%c ", c);
}

CHESS_GAME_MESSAGE gamePrintBoard(ChessGameManager* src)
{
	if (src == NULL)
		return CHESS_GAME_INVALID_ARGUMENT;

	for (int j = 7; j >= 0; j--)
	{
		printf("%d| ", j + 1);
		for (int i = 0; i < 8; i++)
		{
			printPiece(gameGetPieceAt(src, newPos(i, j)));
		}

		printf("|\n");
	}

	printf("  -----------------\n");
	printf("   A B C D E F G H\n");

	return CHESS_GAME_SUCCESS;
}

ChessPiece* gameGetPieceAt(ChessGameManager* src, Position pos)
{
	if (posIsInBoard(pos) == false)
		return NULL;

	return src->game->gameBoard[pos.x][pos.y];
}

void gameSetPieceAt(ChessGameManager* src, Position pos, ChessPiece* newPiece)
{
	if (posIsInBoard(pos) == false)
		return;

	src->game->gameBoard[pos.x][pos.y] = newPiece;
}

void gameMovePiece(ChessGameManager* src, Move move)
{
	ChessPiece* fromPiece = gameGetPieceAt(src, move.from);
	ChessPiece* toPiece = gameGetPieceAt(src, move.to);

	if (toPiece != NULL)
		free(toPiece);

	fromPiece->position = move.to;
	gameSetPieceAt(src, move.to, fromPiece);
	gameSetPieceAt(src, move.from, NULL);
}

#pragma endregion

#pragma region Piece Valid Functions (not in header)

bool checkClearPath(ChessGameManager* src, Move move)
{
	Position step = posNormilized(posDiff(move.from, move.to));

	// check that there is no piece between move.from and move.to
	// start at move.from, go one step at a time until move.to
	for (Position p = posAdd(move.from, step); posEqual(p, move.to) == false; p = posAdd(p, step))
	{
		if (gameGetPieceAt(src, p) != NULL)
			return false;
	}

	return true;
}

bool isValidMovePawn(ChessGameManager* src, Move move)
{
	return true;
}

bool isValidMoveKnight(ChessGameManager* src, Move move)
{
	Position absDiff = posAbs(posDiff(move.from, move.to));

	return (absDiff.x == 2 && absDiff.y == 1) ||
		(absDiff.x == 1 && absDiff.y == 2);
}

bool isValidMoveBishop(ChessGameManager* src, Move move)
{
	Position diffAbs = posAbs(posDiff(move.from, move.to));

	// check if moving in a valid diagonal
	if (diffAbs.x != diffAbs.y)
		return false;

	return checkClearPath(src, move);
}

bool isValidMoveRook(ChessGameManager* src, Move move)
{
	Position diff = posDiff(move.from, move.to);

	// check if moving in only one axis (the case both are zero is detected before)
	if (diff.x != 0 && diff.y != 0)
		return false;

	return checkClearPath(src, move);
}

bool isValidMoveQueen(ChessGameManager* src, Move move)
{
	return isValidMoveBishop(src, move) || isValidMoveRook(src, move);
}

bool isValidMoveKing(ChessGameManager* src, Move move)
{
	Position absDiff = posAbs(posDiff(move.from, move.to));
	return absDiff.x <= 1 && absDiff.x <= 1;
}

typedef bool(*ValidFuncPtr)(ChessGameManager*, Move);
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

bool logicCheckThreatened(ChessGameManager* src, Position pos, PLAYER_COLOR currColor)
{
	ChessGameManager* copy = gameCopy(src); // copy game to overide piece at pos, without changing the game
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

void logicUpdateGameStatus(ChessGameManager* src)
{
}

IS_VALID_MOVE_RESULT logicIsValidMove(ChessGameManager* src, Move move)
{
	ChessPiece* fromPiece = gameGetPieceAt(src, move.from);
	ChessPiece* toPiece = gameGetPieceAt(src, move.to);

	if (posIsInBoard(move.from) == false || posIsInBoard(move.to) == false)
		return IVMR_INVALID_POSITION;

	if (fromPiece == NULL)
		return IVMR_NO_PIECE_IN_POS;

	if (toPiece != NULL && fromPiece->color == toPiece->color)
		return IVMR_ILLEGAL_MOVE;

	if (posEqual(move.from, move.to))
		return IVMR_ILLEGAL_MOVE;

	if (getValidFuncPtr(fromPiece->type)(src, move) == false)
		return IVMR_ILLEGAL_MOVE;

	ChessPiece* king = ((src->game->currentPlayer == WHITE) ? src->game->whiteKing : src->game->blackKing);
	logicCheckThreatened(src, king->position, src->game->currentPlayer);
	//return IVMR_KING_STILL_THREATENED;
	//return IVMR_KING_GET_THREATENED;

	return IVMR_VALID;
}

#pragma endregion