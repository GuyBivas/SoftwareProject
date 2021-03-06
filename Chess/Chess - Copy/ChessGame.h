#ifndef ChessGame_H_
#define ChessGame_H_
#include <stdbool.h>
#include "Enums.h"
#include "MoveOptionsList.h"


//Definitions
#define _FIAR_GAME_PLAYER_1_SYMBOL 'X'
#define _FIAR_GAME_PLAYER_2_SYMBOL 'O'

ChessGame* gameCreate(int historySize);

ChessGame* gameCopy(ChessGame* src);

void gameDestroy(ChessGame* src);

MoveOptionsList gameGetMoves(ChessGame* src, Position pos);

CHESS_GAME_MESSAGE gameMakeMove(ChessGame* src, Move move);

CHESS_GAME_MESSAGE gameUndoPrevMove(ChessGame* src);

CHESS_GAME_MESSAGE gamePrintBoard(ChessGame* src); // Move to console mode?

#endif