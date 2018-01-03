#include <stdio.h>
#include <stdlib.h>
#include "SPMinimax.h"
#include "limits.h"
#include "SPMinimaxNode.h"
#include "SPFIARGame.h"
#include "SPMainAux.h"

int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth)
{
	bool mallocError = false;
	Node* root = createRoot(currentGame);
	if (root == NULL)
	{
		printf("Error: spMinimaxSuggestMove has failed");
		return -1;
	}

	createTree(root, maxDepth, &mallocError);

	if (mallocError)
	{
		destroyTree(root, true);
		printf("Error: spMinimaxSuggestMove has failed");
		return -1;
	}

	applyMinimax(root);

	for (int i = 0; i < root->childrenCount; i++)
	{
		if (root->score == root->children[i]->score)
		{
			int result = root->children[i]->colIndex;
			destroyTree(root, false);
			return result;
		}
	}

	return -1; // shouldt get here
}

Node* createRoot(SPFiarGame* game)
{
	Node* root = (Node*)malloc(sizeof(Node));

	if (root == NULL)
		return NULL;

	root->game = game;
	root->isMaxNode = (game->currentPlayer == PLAYER_2_SYMBOL); // true when comp;

	return root;
}

void createTree(Node* node, unsigned int maxDepth, bool* mallocError)
{
	int childrenIndex = 0; // for later
	int freeCols = COLUMNS - fullColumnsCount(node->game);

	if (maxDepth == 0) // TODO: maybe 1
	{
		node->childrenCount = 0;
		node->children = NULL;
		return;
	}

	node->childrenCount = freeCols;
	node->children = (Node**)malloc(sizeof(Node*) * freeCols);

	if (node->children == NULL)
	{
		node->childrenCount = 0;
		*mallocError = true;
		return;
	}

	for (int i = 0; i < COLUMNS; i++)
	{
		if (!spFiarGameIsValidMove(node->game, i))
			continue;

		Node* son = (Node*)malloc(sizeof(Node));
		SPFiarGame* gameCopy = spFiarGameCopy(node->game);

		if (son == NULL || gameCopy == NULL)
		{
			if (son != NULL)
				free(son);

			if (gameCopy != NULL)
				free(gameCopy);

			*mallocError = true;
			return;
		}

		spFiarGameSetMove(gameCopy, i);

		son->colIndex = i;
		son->game = gameCopy;
		son->isMaxNode = !node->isMaxNode;

		if (spFiarCheckWinner(son->game) == '\0')
		{
			createTree(son, maxDepth - 1, mallocError);
		}
		else
		{
			son->childrenCount = 0;
			son->children = NULL;
		}

		node->children[childrenIndex++] = son;
	}
}

void destroyTree(Node* node, bool includeRoot)
{
	if (node == NULL)
		return;

	if (includeRoot)
		free(node->game);

	if (node->childrenCount > 0)
	{
		for (int i = 0; i < node->childrenCount; i++)
		{
			destroyTree(node->children[i], false);
		}

		free(node->children);
	}
}

int getScore(SPFiarGame* currentGame)
{
	int score = 0;
	int hist[9];
	int weightVector[] = { 0, -5, -2, -1, 0, 1, 2, 5, 0 };

	for (int i = 0; i < 9; i++)
		hist[i] = 0;

	calcHist(currentGame, hist);

	if (hist[0] > 0)
	{
		score = INT_MIN;
	}
	else if (hist[8] > 0)
	{
		score = INT_MAX;
	}
	else
	{
		for (int i = 0; i < 9; i++)
			score += weightVector[i] * hist[i];
	}

	score = inverseNumber(score);
	return score;

	/*if (currentGame->currentPlayer == PLAYER_1_SYMBOL)
		return score;
	else
		return inverseNumber(score);*/
}

void calcHist(SPFiarGame* currentGame, int* hist)
{
	scanVerticalBlocks(currentGame, hist);
	scanHorizontalBlocks(currentGame, hist);
	scanDiag1Blocks(currentGame, hist);
	scanDiag2Blocks(currentGame, hist);
}

void scanVerticalBlocks(SPFiarGame* src, int* hist)
{
	char arr[4];
	int blockScore;

	for (int i = 0; i < ROWS - 3; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				arr[k] = src->gameBoard[i + k][j];
			}

			blockScore = calcBlockScore(arr);
			hist[blockScore + 4]++;
		}
	}
}

void scanHorizontalBlocks(SPFiarGame* src, int* hist)
{
	char arr[4];
	int blockScore;

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS - 3; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				arr[k] = src->gameBoard[i][j + k];
			}

			blockScore = calcBlockScore(arr);
			hist[blockScore + 4]++;
		}
	}
}

void scanDiag1Blocks(SPFiarGame* src, int* hist)
{
	char arr[4];
	int blockScore;

	for (int i = 0; i < ROWS - 3; i++)
	{
		for (int j = 0; j < COLUMNS - 3; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				arr[k] = src->gameBoard[i + k][j + k];
			}

			blockScore = calcBlockScore(arr);
			hist[blockScore + 4]++;
		}
	}
}

void scanDiag2Blocks(SPFiarGame* src, int* hist)
{
	char arr[4];
	int blockScore;

	for (int i = 0; i < ROWS - 3; i++)
	{
		for (int j = 3; j < COLUMNS; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				arr[k] = src->gameBoard[i + k][j - k];
			}

			blockScore = calcBlockScore(arr);
			hist[blockScore + 4]++;
		}
	}
}

int inverseNumber(int num)
{
	if (num == INT_MIN)
		return INT_MAX;
	else if (num == INT_MAX)
		return INT_MIN;
	else
		return -num;
}

int calcBlockScore(char* arr)
{
	int score = 0;

	for (int i = 0; i < 4; i++)
	{
		if (arr[i] == PLAYER_1_SYMBOL)
			score++;
		else if (arr[i] == PLAYER_2_SYMBOL)
			score--;
	}

	return score;
}