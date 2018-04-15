#ifndef CHESSGAMEMANAGER_H
#define CHESSGAMEMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "Enums.h"
#include "ChessGame.h"
#include "HistoryCircularArray.h"

typedef struct chess_game_manager {
	ChessGame* game;
	HistoryCircularArray* history;
	PLAYER_MODE mode;
	GAME_DIFFICULTY difficulty;
	PLAYER_COLOR computerColor;
} ChessGameManager;

ChessGameManager* gameManagerCreate(int historySize);
void gameManagerDestroy(ChessGameManager* src);
void gameManagerMakeMove(ChessGameManager* src, Move move);
void gameManagerUndoPrevMove(ChessGameManager* src);
CHESS_GAME_MESSAGE gameManagerPrintBoard(ChessGameManager* src); // Move to console mode?


#endif