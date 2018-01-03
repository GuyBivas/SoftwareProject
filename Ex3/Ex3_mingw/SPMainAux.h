#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SPFIARGame.h"
#include "SPFIARParser.h"
#include "SPMinimax.h"
#include "SPMinimaxNode.h"

#define SPAN 4
#define ROWS 6
#define COLUMNS 7
#define PLAYER_1_SYMBOL 'X'
#define PLAYER_2_SYMBOL 'O'
#define TIE_SYMBOL '-'
#define EMPTY_ENTRY ' '

#define HISTORY_SIZE 20

//SPFIARGame
char getOtherPlayer(SPFiarGame* src);
bool isBoardFull(SPFiarGame* src);
int fullColumnsCount(SPFiarGame* src);

//SPMinimax
Node* createRoot(SPFiarGame* game);
void createTree(Node* node, unsigned int maxDepth, bool* mallocError);
void destroyTree(Node* node, bool includeRoot);

int getScore(SPFiarGame* currentGame);
void calcHist(SPFiarGame* currentGame, int* hist);
void scanVerticalBlocks(SPFiarGame* src, int* hist);
void scanHorizontalBlocks(SPFiarGame* src, int* hist);
void scanDiag1Blocks(SPFiarGame* src, int* hist);
void scanDiag2Blocks(SPFiarGame* src, int* hist);
int inverseNumber(int num);
int calcBlockScore(char* arr);


//////

extern bool mallocError;

void printWinner(SPFiarGame* curGame);
void endGame(SPFiarGame * game, bool isMemError);
char* my_strdup_main(const char *s);
int doUserCommand(SPFiarGame* curGame, int maxDepth);
SPCommand readCommand(SPFiarGame *curGame);
int getMaxDepth();
void computerTurn(SPFiarGame* curGame, int maxDepth);

/////

#endif