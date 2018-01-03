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

	for (int i = 0; i < COLUMNS; i++)
	{
		game->tops[i] = 0;
	}

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			game->gameBoard[i][j] = ' ';
		}
	}

	game->currentPlayer = PLAYER_1_SYMBOL;

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

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			newGame->gameBoard[i][j] = src->gameBoard[i][j];
		}
	}

	for (int k = 0; k < COLUMNS; k++)
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

	free(src->history);
	free(src);
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col)
{
	if (src == NULL || col > COLUMNS - 1 || col < 0)
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
	return src->tops[col] < ROWS;
}

// review
SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src)
{
	int rowNum, colNum, k;
	if (src == NULL)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	if (src->history->actualSize == 0)
		return SP_FIAR_GAME_NO_HISTORY;

	if (spFiarCheckWinner(src) == SP_FIAR_GAME_PLAYER_1_SYMBOL) //checks if the undo made after player's win.
		k = 1;
	else
		k = 2;


	for (int i = 0; i < k; i++)
	{
		colNum = spArrayListGetLast(src->history);
		rowNum = src->tops[colNum] - 1;

		src->gameBoard[rowNum][colNum] = '0';
		spArrayListRemoveLast(src->history);
		src->tops[colNum] -= 1;
	}
	if (k == 1) // in case that the undo is after player's win.
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;


	return SP_FIAR_GAME_SUCCESS;

}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src)
{
	printf("\n"); // TODO: maybe remove

	if (src == NULL)
		return SP_FIAR_GAME_INVALID_ARGUMENT;

	for (int i = 0; i < ROWS; i++)
	{
		printf("| ");
		for (int j = 0; j < COLUMNS; j++)
		{
			printf("%c ", src->gameBoard[ROWS - i - 1][j]);
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

	//TODO: do we need to check if the game is over?

	return src->currentPlayer;
}

char spFiarCheckWinner(SPFiarGame* src)
{
	int score = getScore(src);

	if (score == INT_MAX)
	{
		return src->currentPlayer;
	}
	else if (score == INT_MIN)
	{
		return getOtherPlayer(src);
	}
	else if (isBoardFull(src))
	{
		return TIE_SYMBOL;
	}

	return '\0';
}

char getOtherPlayer(SPFiarGame* src)
{
	return (src->currentPlayer == PLAYER_1_SYMBOL) ? PLAYER_2_SYMBOL : PLAYER_1_SYMBOL;
}

bool isBoardFull(SPFiarGame* src)
{
	return fullColumnsCount(src) == COLUMNS;
}

int fullColumnsCount(SPFiarGame* src)
{
	int result = 0;

	for (int i = 0; i < COLUMNS; i++)
		if (src->tops[i] == ROWS)
			result++;

	return result;
}