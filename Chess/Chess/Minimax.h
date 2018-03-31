#ifndef MINIMAX_H
#define MINIMAX_H

#include "ChessGame.h"

#define MATE_SCORE 1000
#define PAWN_SCORE 1
#define KNIGHT_SCORE 3
#define BISHOP_SCORE 3
#define ROOK_SCORE 5
#define QUEEN_SCORE 9
#define KING_SCORE 100

int getScore(ChessGame* game);

#endif