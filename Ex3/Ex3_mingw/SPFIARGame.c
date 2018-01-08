#include <stdio.h>
#include <stdlib.h>
#include "SPFIARGame.h"
#include "SPArrayList.h"
#include "SPMinimax.h"
#include "limits.h"
#include "SPMainAux.h"

SPFiarGame* spFiarGameCreate(int historySize)
{
	if (historySize <= 0)
		return NULL;

	SPFiarGame* game = (SPFiarGame*)malloc(sizeof(SPFiarGame));
	if (game == NULL)
		return NULL;

	game->history = spArrayListCreate(historySize);

	if (game->history == NULL)
	{
		free(game);
		return NULL;
	}

	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
	{
		game->tops[i] = 0;
	}

	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++)
	{
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++)
		{
			game->gameBoard[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;
		}
	}

	game->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;

	return game;
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src)
{
	SPFiarGame* newGame = (SPFiarGame*)malloc(sizeof(SPFiarGame));

	if (newGame == NULL)
	{
		printf("Error: spFiarGameCopy has failed");
		return NULL;
	}

	newGame->currentPlayer = src->currentPlayer;

	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++)
	{
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++)
		{
			newGame->gameBoard[i][j] = src->gameBoard[i][j];
		}
	}

	for (int k = 0; k < SP_FIAR_GAME_N_COLUMNS; k++)
		newGame->tops[k] = src->tops[k];

	newGame->history = spArrayListCopy(src->history);

	if (newGame->history == NULL)
	{
		free(newGame);
		return NULL;
	}

	return newGame;
}

void spFiarGameDestroy(SPFiarGame* src)
{
	if (src == NULL)
		return;

	spArrayListDestroy(src->history);
	//free(src->history);
	free(src);
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col)
{
	if (src == NULL || col > SP_FIAR_GAME_N_COLUMNS - 1 || col < 0)
		return SP_FIAR_GAME_INVALID_ARGUMENT;

	if (spFiarGameIsValidMove(src, col) == false)
		return SP_FIAR_GAME_INVALID_MOVE;

	src->gameBoard[src->tops[col]][col] = src->currentPlayer;
	src->tops[col]++;
	src->currentPlayer = getOtherPlayer(src);

	if (spArrayListIsFull(src->history))
	{
		spArrayListRemoveFirst(src->history);
	}

	spArrayListAddLast(src->history, col);

	return SP_FIAR_GAME_SUCCESS;
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col)
{
	if (src == NULL)
		return false;

	return src->tops[col] < SP_FIAR_GAME_N_ROWS;
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src)
{
	int rowNum, colNum;

	if (src == NULL)
	{
		printf("Error: cannot undo previous move!\n");
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}

	if (src->history->actualSize == 0)
	{
		printf("Error: cannot undo previous move!\n");
		return SP_FIAR_GAME_NO_HISTORY;
	}

	if (src->history->actualSize == 1)
	{
		char symbol;
		colNum = spArrayListGetLast(src->history);
		rowNum = src->tops[colNum] - 1;

		symbol = src->gameBoard[rowNum][colNum];
		if (symbol == SP_FIAR_GAME_PLAYER_2_SYMBOL)
		{
			printf("Error: cannot undo previous move!\n");
			return SP_FIAR_GAME_NO_HISTORY;
		}
	}

	char symbol;
	colNum = spArrayListGetLast(src->history);
	rowNum = src->tops[colNum] - 1;

	symbol = src->gameBoard[rowNum][colNum];
	src->gameBoard[rowNum][colNum] = SP_FIAR_GAME_EMPTY_ENTRY;
	spArrayListRemoveLast(src->history);
	src->tops[colNum]--;

	if (symbol == SP_FIAR_GAME_PLAYER_2_SYMBOL)
	{
		printf("Remove disc: remove computer's disc at column %d\n", colNum + 1);
	}
	else if (symbol == SP_FIAR_GAME_PLAYER_1_SYMBOL)
	{
		printf("Remove disc: remove user's disc at column %d\n", colNum + 1);
	}


	src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;

	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src)
{
	if (src == NULL)
		return SP_FIAR_GAME_INVALID_ARGUMENT;

	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++)
	{
		printf("| ");
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++)
		{
			printf("%c ", src->gameBoard[SP_FIAR_GAME_N_ROWS - i - 1][j]);
		}

		printf("|\n");
	}

	printf("-----------------\n");
	printf("  1 2 3 4 5 6 7  \n");

	return SP_FIAR_GAME_SUCCESS;
}

char spFiarGameGetCurrentPlayer(SPFiarGame* src)
{
	if (src == NULL)
		return SP_FIAR_GAME_EMPTY_ENTRY;

	return src->currentPlayer;
}

char spFiarCheckWinner(SPFiarGame* src)
{
	int score = getScore(src);

	if (score == INT_MAX)
	{
		return SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
	else if (score == INT_MIN)
	{
		return SP_FIAR_GAME_PLAYER_2_SYMBOL;
	}
	else if (isBoardFull(src))
	{
		return SP_FIAR_GAME_TIE_SYMBOL;
	}

	return '\0';
}

char getOtherPlayer(SPFiarGame* src)
{
	return (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) ? SP_FIAR_GAME_PLAYER_2_SYMBOL : SP_FIAR_GAME_PLAYER_1_SYMBOL;
}

bool isBoardFull(SPFiarGame* src)
{
	return fullColumnsCount(src) == SP_FIAR_GAME_N_COLUMNS;
}

int fullColumnsCount(SPFiarGame* src)
{
	int result = 0;

	if (src == NULL)
		return 0;

	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
		if (src->tops[i] == SP_FIAR_GAME_N_ROWS)
			result++;

	return result;
}