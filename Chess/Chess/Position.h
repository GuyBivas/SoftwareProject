#ifndef POSITION_H
#define POSITION_H

#define absVal(a) (((a) > (0)) ? (a) : (-a))

#include <stdbool.h>
#include "Enums.h"

typedef struct position {
	int x; // row
	int y; // col
} Position;

typedef struct move {
	Position from;
	Position to;
} Move;

// sometimes treated as Vector2
typedef Position Vector2;

Position newPos(int x, int y);
Move newMove(Position from, Position to);
bool posEqual(Position pos1, Position pos2);
bool posIsInBoard(Position pos);

Vector2 vecAdd(Vector2 vec1, Vector2 vec2);
Vector2 vecDiff(Vector2 vec1, Vector2 vec2);
Vector2 vecAbs(Vector2 vec);
Vector2 vecNormilized(Vector2 vec);
int vecMagnitude(Vector2 vec);

#endif