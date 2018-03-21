#include <stdio.h>
#include <stdlib.h>
#include "ChessLogic.h"

bool logicCheckThreatened(ChessGame* src, Position pos, PLAYER_COLOR currColor)
{
	ChessPiece dummyPieace;
	dummyPieace.color = currColor;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++) 
		{
			ChessPiece* checkedPieace = src->gameBoard[i][j];
			Move move;
			Position p;
			
			p.y = i;
			p.x = j;

			move.from = p;
			move.to = pos;

			if (checkedPieace != NULL && checkedPieace->color != currColor)
			{
				if (src->gameBoard[pos.y][pos.x] == NULL)
					src->gameBoard[pos.y][pos.x] = &dummyPieace;

				if (logicIsValidMove(src, move)) 
				{
					if (src->gameBoard[pos.y][pos.x] == &dummyPieace)
						src->gameBoard[pos.y][pos.x] = NULL;

					return true;
				}
			}
		}
	}

	return false;
}

void logicUpdateGameStatus(ChessGame * src)
{
}

bool logicIsValidMove(ChessGame* src, Move move)
{
	return false;
}