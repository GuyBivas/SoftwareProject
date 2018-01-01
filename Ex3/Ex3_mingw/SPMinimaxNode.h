#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

#include <stdbool.h>
#include "SPFIARGame.h"

typedef struct Node {
	SPFiarGame* game;
	struct Node** children;
	int childrenCount;
	bool isMaxNode;
	int score;
	int colIndex;
} Node;

void applyMinimax(Node* node);

#endif