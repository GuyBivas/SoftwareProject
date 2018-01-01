#include "SPMinimax.h"
#include "limits.h"
#include "SPMinimaxNode.h"

void applyMinimax(Node* node)
{
	if (node->childrenCount == 0)
	{
		node->score = getScore(node->game);
		return;
	}

	for (int i = 0; i < node->childrenCount; i++) 
	{
		applyMinimax(node->children[i]);
	}

	node->score = node->children[0]->score;
	for (int i = 1; i < node->childrenCount; i++) 
	{
		node->score = getMinOrMax(node->score, node->children[i]->score, node->isMaxNode);
	}
}

int getMinOrMax(int a, int b, bool takeMax) 
{
	if (a > b)
		return takeMax ? a : b;
	else
		return takeMax ? b : a;
}