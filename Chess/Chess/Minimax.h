#ifndef MINIMAX_H
#define MINIMAX_H

#include <stdlib.h>
#include <stdbool.h>
#include "Enums.h"
#include "Position.h"
#include "ChessGame.h"
#include "MoveOptionsList.h"

#define MATE_SCORE 1000
#define INF_SCORE 10000

#define PAWN_SCORE 1
#define KNIGHT_SCORE 3
#define BISHOP_SCORE 3
#define ROOK_SCORE 5
#define QUEEN_SCORE 9
#define KING_SCORE 100

Move minimaxSuggestMove(ChessGame* game, int depth);
int getGameScore(ChessGame* game);

#endif