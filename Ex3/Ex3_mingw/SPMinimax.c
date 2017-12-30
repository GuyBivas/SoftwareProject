#include "SPMinimax.h"
#include "limits.h"
#include "SPMinimaxNode.h"

int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth)
{
	Node* root = createRoot(currentGame);
	bool mallocError;

	createTree(root, maxDepth, &mallocError);

	if (mallocError)
		destroyTree(root);

	applyMinimax(root);
	return root->colIndex;
}

Node* createRoot(SPFiarGame* game)
{
	Node* root = (Node*)malloc(sizeof(Node));
	root->game = game;
	root->isMaxNode = true;

	return root;
}

bool createTree(Node* node, unsigned int maxDepth, bool* mallocError)
{

}

void destroyTree(Node* node)
{

}

int getScore(SPFiarGame* currentGame)
{
	int score = 0;
	int hist[9];
	int weightVector[] = { 0, -5, -2, -1, 0, 1, 2, 5, 0 };

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

	if (currentGame->currentPlayer == PLAYER_1_SYMBOL)
		return score;
	else
		return inverseNumber(score);
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

	for (int i = 3; i < ROWS; i++)
	{
		for (int j = 3; j < COLUMNS; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				arr[k] = src->gameBoard[i - k][j - k];
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