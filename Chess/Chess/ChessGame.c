#include <stdio.h>
#include <stdlib.h>
#include "ChessGame.h"
#include "ChessLogic.h"
#include "MoveOptionsList.h"

MoveOptionsList* gameGetValidMoves(ChessGame* src, Position pos)
{
	MoveOptionsList* validMoves = arrayListCreate(8 * 4 - 3); // 8*4-3 is the maximum possible moves for a queen
	ChessPiece* piece = src->gameBoard[pos.y][pos.x];

	if (piece == NULL)
		return validMoves;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position p;
			Move move;

			p.y = i;
			p.x = j;

			move.from = pos;
			move.to = p;
			if (logicIsValidMove(src, move))
			{
				moveOption* moveOpt = (moveOption*)malloc(sizeof(moveOption));
				moveOpt->pos = p;
				moveOpt->isCapturing = (src->gameBoard[i][j] != NULL); // move to a pos of other pieace is always capturing (must be other color if valid)
				moveOpt->isThreatened = logicCheckThreatened(src, p, piece->color);
				arrayListAddLast(validMoves, moveOpt);
			}
		}
	}

	return validMoves;
}