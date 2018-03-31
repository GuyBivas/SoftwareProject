#ifndef CHESSGAMEMANAGER_H
#define CHESSGAMEMANAGER_H

#include <stdlib.h>
#include <stdbool.h>
#include "Enums.h"
#include "ChessGame.h"
#include "HistoryCircularArray.h"

typedef struct chess_game_manager {
	ChessGame* game;
	HistoryCircularArray* history;
	PLAYER_MODE mode;
	GAME_DIFFICULTY difficulty;
} ChessGameManager;

ChessGameManager* gameManagerCreate(int historySize);
void gameManagerDestroy(ChessGameManager* src);
bool gameManagerMakeMove(ChessGameManager* src, Move move);
CHESS_GAME_MESSAGE gameManagerUndoPrevMove(ChessGameManager* src);
CHESS_GAME_MESSAGE gameManagerPrintBoard(ChessGameManager* src); // Move to console mode?

#endif