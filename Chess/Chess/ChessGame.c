
#include <stdio.h>
#include <stdlib.h>
#include "ChessGame.h"
#include "ChessLogic.h"
#include "MoveOptionsList.h"
#include "HistoryCircularArray.h"

MoveOptionsList* gameGetValidMoves(ChessGame* src, Position pos)
{
	MoveOptionsList* validMoves = arrayListCreate(8 * 4 - 3); // 8*4-3 is the maximum possible moves for a queen
	ChessPiece* piece = src->gameBoard[pos.y][pos.x];

	if (piece == NULL)
		return validMoves;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position p;
			Move move;

			p.y = i;
			p.x = j;

			move.from = pos;
			move.to = p;
			if (logicIsValidMove(src, move))
			{
				moveOption* moveOpt = (moveOption*)malloc(sizeof(moveOption));
				moveOpt->pos = p;
				moveOpt->isCapturing = (src->gameBoard[i][j] != NULL); // move to a pos of other pieace is always capturing (must be other color if valid)
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

	ChessGame* game = (ChessGame*)calloc(sizeof(ChessGame));

	if (game == NULL)
		return NULL;

	game->history = circularArrayCreate(historySize);

	if (game->history != NULL)
	{
		free(game);
		return NULL;
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			game->gameBoard[i][j] = NULL;
			if ((i >= 0 && i <= 1) || (i >= 6 && i <= 7))
			{
				game->gameBoard[i][j] = (ChessPiece*)malloc(sizeof(ChessPiece));
				game->gameBoard[i][j]->position.x = j;
				game->gameBoard[i][j]->position.y = i;
				if (i == 1 || i == 2)
					game->gameBoard[i][j]->color = WHITE;
				else
					game->gameBoard[i][j]->color = BLACK;


				if (i == 0 || i == 7)
				{
					if (j == 0 || j == 7)
						game->gameBoard[i][j]->type = ROOK;
					else if (j == 1 || j == 6)
						game->gameBoard[i][j]->type = KNIGHT;
					else if (j == 2 || j == 5)
						game->gameBoard[i][j]->type = BISHOP;
					else if (j == 3)
						game->gameBoard[i][j]->type = QUEEN;
					else
					{
						game->gameBoard[i][j]->type = KING;
						if (game->gameBoard[i][j]->color == WHITE)
							game->whiteKing = game->gameBoard[i][j];
						else
							game->blackKing = game->gameBoard[i][j];
					}
				}
				else
					game->gameBoard[i][j]->type = PAWN;

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
	if (piece == NULL)
		printf("_");
	if (piece->type == PAWN)
	{
		if (piece->color == WHITE)
			printf("p");
		else
			printf("P");
	}
	if (piece->type == KNIGHT)
	{
		if (piece->color == WHITE)
			printf("n");
		else
			printf("N");
	}
	if (piece->type == ROOK)
	{
		if (piece->color == WHITE)
			printf("r");
		else
			printf("R");
	}
	if (piece->type == QUEEN)
	{
		if (piece->color == WHITE)
			printf("q");
		else
			printf("Q");
	}
	if (piece->type == KING)
	{
		if (piece->color == WHITE)
			printf("k");
		else
			printf("K");
	}
	if (piece->type == BISHOP)
	{
		if (piece->color == WHITE)
			printf("b");
		else
			printf("B");
	}
}



CHESS_GAME_MESSAGE gamePrintBoard(ChessGame* src)
{
	if (src == NULL)
		return CHESS_GAME_INVALID_ARGUMENT;

	for (int i = 0; i < 8; i++)
	{
		printf("%d| ", i);
		for (int j = 0; j < 8; j++)
		{
			printPiece(src->gameBoard[8 - i - 1][j]);
		}

		printf("|\n");
	}

	printf("-----------------\n");
	printf("   A B C D E F G H\n");

	return CHESS_GAME_SUCCESS;
}






CHESS_GAME_MESSAGE gameMakeMove(ChessGame* src, Move move)
{
	ChessGame* copy = gameCopy(src);

	if (!(gameIsValidMove(src, move)))
		return CHESS_GAME_INVALID_MOVE;

	circularArrayAdd(src->history, copy);

	if (src->gameBoard[move.to.y][move.to.x] != NULL)//delete captured piece
		free(src->gameBoard[move.to.y][move.to.x]);


	src->gameBoard[move.to.y][move.to.x] = src->gameBoard[move.from.y][move.from.x];
	src->gameBoard[move.to.y][move.to.x]->position.y = move.to.y;
	src->gameBoard[move.to.y][move.to.x]->position.x = move.to.x;
	src->gameBoard[move.from.y][move.from.x] = NULL;
	src->currentPlayer = 1 - (src->currentPlayer);

	return CHESS_GAME_SUCCESS;
}



bool gameIsValidMove(ChessGame * src, Move move)
{
	return false;
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
