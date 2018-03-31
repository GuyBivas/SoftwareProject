#ifndef POSITION_H
#define POSITION_H

#include <stdbool.h>
#include "Enums.h"

// sometimes treated as Vector2
typedef struct position {
	int x;
	int y;
} Position;

typedef struct move {
	Position from;
	Position to;
} Move;

Position newPos(int x, int y);
Move newMove(Position from, Position to);
bool posEqual(Position pos1, Position pos2);
Position posAdd(Position pos1, Position pos2);
Position posDiff(Position pos1, Position pos2); // use Position as Vector2 in return value
Position posAbs(Position pos); // use Position as Vector2 in return value
Position posNormilized(Position pos); // use Position as Vector2 in return value
bool posIsInBoard(Position pos);

#endif