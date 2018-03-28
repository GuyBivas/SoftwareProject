#include <stdio.h>
#include <stdlib.h>
#include "ChessLogic.h"

bool logicCheckThreatened(ChessGame* src, Position pos, PLAYER_COLOR currColor)
{
	ChessPiece* tmp = gameGetPieceAt(src, pos);
	ChessPiece dummyPieace;
	dummyPieace.color = currColor;

	// replace piece at pos to a dummy piece with currColor.
	// neccessary for logicIsValidMove to be correct (the color must be currColor, and also for pawns)
	gameSetPieceAt(src, pos, &dummyPieace);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position p = newPos(i, j);
			
			if (gameGetPieceAt(src, p) != NULL && gameGetPieceAt(src, p)->color != currColor) // if there is an oponent's pieace at p 
			{
				if (logicIsValidMove(src, newMove(p, pos))) // if the oponent's pieace can eat dummyPiece
				{
					gameSetPieceAt(src, pos, tmp); // restore previous piece before return
					return true;
				}
			}
		}
	}

	gameSetPieceAt(src, pos, tmp);// restore previous piece before return
	return false;
}

void logicUpdateGameStatus(ChessGame * src)
{
}

bool logicIsValidMove(ChessGame* src, Move move)
{
	return false;
}

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

bool gameIsInBoard(Position pos)
{
	return !(pos.x >= 8 || pos.y >= 8 || pos.x < 0 || pos.y < 0);
}

ChessPiece* gameGetPieceAt(ChessGame* src, Position pos)
{
	if (gameIsInBoard(pos) == false)
		return NULL;

	return src->gameBoard[pos.x][pos.y];
}

void gameSetPieceAt(ChessGame* src, Position pos, ChessPiece* newPiece)
{
	if (gameIsInBoard(pos) == false)
		return;

	src->gameBoard[pos.x][pos.y] = newPiece;
}

void gameMovePiece(ChessGame* src, Move move)
{
	ChessPiece* fromPiece = gameGetPieceAt(src, move.from);
	ChessPiece* toPiece = gameGetPieceAt(src, move.to);

	if (toPiece != NULL)
		free(toPiece);

	fromPiece->position = move.to;
	gameSetPieceAt(src, move.to, fromPiece);
	gameSetPieceAt(src, move.from, NULL);
}