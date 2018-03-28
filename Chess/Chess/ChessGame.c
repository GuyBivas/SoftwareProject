#include <stdio.h>
#include <stdlib.h>
#include "ChessGame.h"
#include "ChessLogic.h"
#include "MoveOptionsList.h"
#include "HistoryCircularArray.h"

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
			
			if (logicIsValidMove(src, newMove(pos, p)))
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

ChessGame* gameCreate(int historySize)
{
	if (historySize <= 0) //TODO: check if needed
		return NULL;

	ChessGame* game = (ChessGame*)malloc(sizeof(ChessGame));

	if (game == NULL)
		return NULL;

	game->status = STATUS_NORMAL;
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