#include <math.h>
#include "ChessGame.h"

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

Position posAdd(Position pos1, Position pos2)
{
	return newPos(pos1.x + pos2.x, pos1.y + pos2.y);
}

Position posDiff(Position pos1, Position pos2)
{
	int diffX = pos1.x - pos2.x;
	int diffY = pos1.y - pos2.y;

	return newPos(diffX, diffY);
}

Position posAbs(Position pos)
{
	return newPos(abs(pos.x), abs(pos.y));
}

Position posNormilized(Position pos)
{
	Position abs = posAbs(pos);

	int normX = (pos.x == 0 ? 0 : pos.x / abs.x);
	int normY = (pos.y == 0 ? 0 : pos.y / abs.y);

	return newPos(normX, normY);
}

bool posIsInBoard(Position pos)
{
	return (pos.x < 8 || pos.y < 8 || pos.x >= 0 || pos.y >= 0);
}