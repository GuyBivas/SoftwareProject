#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ChessGame.h"
#include "MoveOptionsList.h"
#include "HistoryCircularArray.h"

#pragma region Game Functions

ChessGame* gameCreate(int historySize)
{
	if (historySize <= 0) //TODO: check if needed
		return NULL;

	ChessGame* game = (ChessGame*)malloc(sizeof(ChessGame));

	if (game == NULL)
		return NULL;

	game->status = STATUS_NORMAL;
	game->mode = ONE_PLAYER;
	game->difficulty = EASY;
	game->currentPlayer = WHITE;
	game->history = circularArrayCreate(historySize);

	if (game->history == NULL)
	{
		free(game);
		return NULL;
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			game->gameBoard[i][j] = NULL;
			if (j <= 1 || j >= 6)
			{
				Position pos = newPos(i, j);
				ChessPiece* piece = (ChessPiece*)malloc(sizeof(ChessPiece));
				if (piece == NULL)
					return NULL;

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
					else if (i == 4)
						piece->type = QUEEN;
					else if (i == 3)
					{
						piece->type = KING;

						if (piece->color == WHITE)
							game->whiteKing = piece;
						else
							game->blackKing = piece;
					}
				}
			}
		}
	}

	return game;
}

ChessGame* gameCopy(ChessGame* src)
{
	ChessGame* copy = (ChessGame*)malloc(sizeof(ChessGame));

	if (copy == NULL)
		return NULL;

	copy->currentPlayer = src->currentPlayer;
	copy->history = NULL; // copy is only for minimax - dont need to copy history
	copy->status = src->status;
	copy->whiteKing = src->whiteKing;
	copy->blackKing = src->blackKing;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			copy->gameBoard[i][j] = src->gameBoard[i][j];
		}
	}

	return copy;
}

void gameDestroy(ChessGame* src)
{
	if (src == NULL)
		return;

	circularArrayDestroy(src->history);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			free(src->gameBoard[i][j]);
		}
	}

	free(src);
}

MoveOptionsList* gameGetValidMoves(ChessGame* src, Position pos)
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

bool gameMakeMove(ChessGame* src, Move move)
{
	if (logicIsValidMove(src, move) == false)
		return false;

	circularArrayAdd(src->history, gameCopy(src));

	gameMovePiece(src, move);
	logicUpdateGameStatus(src);

	if (src->status == STATUS_NORMAL)
		src->currentPlayer = (src->currentPlayer == WHITE) ? BLACK : WHITE;

	return true;
}

CHESS_GAME_MESSAGE gameUndoPrevMove(ChessGame* src)
{
	ChessGame* copy = gameCopy(circularArrayGetCurrent(src->history)); // to fix incomplete type
	if (src->history->actualSize == 0)
		return CHESS_GAME_NO_HISTORY;

	src->currentPlayer = copy->currentPlayer;
	src->whiteKing = copy->whiteKing;
	src->blackKing = copy->blackKing;
	src->status = copy->status;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			src->gameBoard[i][j] = copy->gameBoard[i][j];
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

CHESS_GAME_MESSAGE gamePrintBoard(ChessGame* src)
{
	if (src == NULL)
		return CHESS_GAME_INVALID_ARGUMENT;

	for (int j = 7; j >=0; j--)
	{
		printf("%d| ", j+1);
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

ChessPiece* gameGetPieceAt(ChessGame* src, Position pos)
{
	if (gameIsInBoard(pos) == false)
		return NULL;

	return src->gameBoard[pos.x][pos.y];
}

void gameSetPieceAt(ChessGame* src, Position pos, ChessPiece* newPiece)
{
	if (gameIsInBoard(pos) == false)
		return;

	src->gameBoard[pos.x][pos.y] = newPiece;
}

void gameMovePiece(ChessGame* src, Move move)
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

bool isValidMovePawn(ChessGame* src, Move move)
{
	return true;
}

bool isValidMoveKnight(ChessGame* src, Move move)
{
	return true;
}

bool isValidMoveBishop(ChessGame* src, Move move)
{
	return true;
}

bool isValidMoveRook(ChessGame* src, Move move)
{
	return true;
}

bool isValidMoveQueen(ChessGame* src, Move move)
{
	return isValidMoveBishop(src, move) || isValidMoveRook(src, move);
}

bool isValidMoveKing(ChessGame* src, Move move)
{
	int diffX = abs(move.from.x - move.to.x);
	int diffY = abs(move.from.y - move.to.y);

	return diffX <= 1 && diffY <= 1;
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
}

#pragma endregion

#pragma region Logic

bool logicCheckThreatened(ChessGame* src, Position pos, PLAYER_COLOR currColor)
{
	ChessGame* copy = gameCopy(src); // copy game to overide piece at pos, without changing the game
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

void logicUpdateGameStatus(ChessGame* src)
{
}

IS_VALID_MOVE_RESULT logicIsValidMove(ChessGame* src, Move move)
{
	ChessPiece* fromPiece = gameGetPieceAt(src, move.from);
	ChessPiece* toPiece = gameGetPieceAt(src, move.to);

	if (gameIsInBoard(move.from) == false || gameIsInBoard(move.to) == false)
		return IVMR_INVALID_POSITION;

	if (fromPiece == NULL)
		return IVMR_NO_PIECE_IN_POS;

	if (toPiece != NULL && fromPiece->color == toPiece->color)
		return IVMR_ILLEGAL_MOVE;

	if (posEqual(move.from, move.to))
		return IVMR_ILLEGAL_MOVE;

	if (getValidFuncPtr(fromPiece->type)(src, move) == false)
		return IVMR_ILLEGAL_MOVE;

	ChessPiece* king = ((src->currentPlayer == WHITE) ? src->whiteKing : src->blackKing);
	logicCheckThreatened(src, king->position, src->currentPlayer);
	//return IVMR_KING_STILL_THREATENED;
	//return IVMR_KING_GET_THREATENED;

	return IVMR_VALID;
}

#pragma endregion

#pragma region Basic Functions

Position newPos(int x, int y)
{
	Position pos;
	pos.x = x;
	pos.y = y;

	return pos;
}

Move newMove(Position from, Position to)
{
	Move move;
	move.from = from;
	move.to = to;

	return move;
}

bool posEqual(Position pos1, Position pos2)
{
	return pos1.x == pos2.x && pos1.y == pos2.y;
}

bool gameIsInBoard(Position pos)
{
	return !(pos.x >= 8 || pos.y >= 8 || pos.x < 0 || pos.y < 0);
}

#pragma endregion