#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <stdbool.h>
#include "Enums.h"
#include "ChessLogic.h"
#include "MoveOptionsList.h"
#include "HistoryCircularArray.h"

ChessGame* gameCreate(int historySize);
ChessGame* gameCopy(ChessGame* src);
void gameDestroy(ChessGame* src);
MoveOptionsList* gameGetValidMoves(ChessGame* src, Position pos);
bool gameMakeMove(ChessGame* src, Move move);
CHESS_GAME_MESSAGE gameUndoPrevMove(ChessGame* src);
CHESS_GAME_MESSAGE gamePrintBoard(ChessGame* src); // Move to console mode?

#endif