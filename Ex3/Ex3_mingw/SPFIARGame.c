#include <stdio.h>
#include <stdlib.h>
#include "SPFIARGame.h"
#include "SPArrayList.h"
#include "SPMinimax.h"
#include "limits.h"

SPFiarGame* spFiarGameCreate(int historySize)
{
	if (historySize <= 0)
		return NULL;

	SPFiarGame* game = (SPFiarGame*)malloc(sizeof(SPFiarGame));
	if (game == NULL)
		return NULL;

	game->history = (SPArrayList*)malloc(sizeof(SPArrayList));
	game->history->elements = (int*)malloc(sizeof(int) * historySize);

	if (game->history == NULL)
		return NULL;

	return game;
}

void spFiarGameDestroy(SPFiarGame* src)
{
	if (src == NULL)
		return;

	free(src->history);
	free(src);
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col)
{
	return src->tops[col] == ROWS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src)
{
	if (src == NULL)
		return SP_FIAR_GAME_INVALID_ARGUMENT;

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			printf("%c", src->gameBoard[i][j]);
		}
	}

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

	return NULL; // "null character - otherwise"
}

char getOtherPlayer(SPFiarGame* src)
{
	return (src->currentPlayer == PLAYER_1_SYMBOL) ? PLAYER_2_SYMBOL : PLAYER_1_SYMBOL;
}

bool isBoardFull(SPFiarGame* src) 
{
	for (int i = 0; i < COLUMNS; i++)
		if (src->tops[i] != ROWS)
			return false;

	return true;
}