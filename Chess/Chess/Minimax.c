#include <stdlib.h>
#include "Minimax.h"

int getPieceScore(ChessPiece* piece) 
{
	int score;

	if (piece == NULL)
		return 0;
	
	switch (piece->type)
	{
	case PAWN:
		score = PAWN_SCORE;
		break;
	case KNIGHT:
		score = KNIGHT_SCORE;
		break;
	case ROOK:
		score = BISHOP_SCORE;
		break;
	case BISHOP:
		score = ROOK_SCORE;
		break;
	case QUEEN:
		score = QUEEN_SCORE;
		break;
	case KING:
		score = KING_SCORE;
		break;
	}

	return piece->color == WHITE ? score : -score;
}

int getScore(ChessGame * game)
{
	int result;
	
	if (game->status == STATUS_MATE)
		return 0;

	if (game->status == STATUS_MATE)
		result = game->currentPlayer == WHITE ? MATE_SCORE : -MATE_SCORE;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			ChessPiece* piece = gameGetPieceAt(game, newPos(i, j));
			result += getPieceScore(piece);
		}
	}

	return result;
}
