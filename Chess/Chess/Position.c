#include "Position.h"

Position newPos(int x, int y)
{
	Position pos;
	pos.x = x;
	pos.y = y;

	return pos;
}

Move newMove(Position from, Position to)
{
	Move move;
	move.from = from;
	move.to = to;

	return move;
}

bool posEqual(Position pos1, Position pos2)
{
	return pos1.x == pos2.x && pos1.y == pos2.y;
}

bool posIsInBoard(Position pos)
{
	return (pos.x < 8 || pos.y < 8 || pos.x >= 0 || pos.y >= 0);
}

Vector2 vecAdd(Vector2 vec1, Vector2 vec2)
{
	return newPos(vec1.x + vec2.x, vec1.y + vec2.y);
}

Vector2 vecDiff(Vector2 vec1, Vector2 vec2)
{
	int diffX = vec1.x - vec2.x;
	int diffY = vec1.y - vec2.y;

	return newPos(diffX, diffY);
}

Vector2 vecAbs(Vector2 vec)
{
	return newPos(abs(vec.x), abs(vec.y));
}

Vector2 vecNormilized(Vector2 vec)
{
	Position abs = vecAbs(vec);

	int normX = (vec.x == 0 ? 0 : vec.x / abs.x);
	int normY = (vec.y == 0 ? 0 : vec.y / abs.y);

	return newPos(normX, normY);
}

int vecMagnitude(Vector2 vec)
{
	Position abs = vecAbs(vec);
	return abs.x + abs.y;
}