#include "Minimax.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))

int minimax(ChessGame* game, int depth, int a, int b, int color, bool returnMove)
{
	if (depth == 0 || game->status == STATUS_DRAW || game->status == STATUS_MATE)
	{
		return getGameScore(game) * color;
	}
	else
	{
		int moveScore, bestMove;
		int bestValue = -INF_SCORE;
		MoveOptionsList* moves = gameGetAllValidMoves(game, game->currentPlayer, false);

		for (int i = 0; i < moves->actualSize; i++)
		{
			ChessGame* copy = gameCopy(game);
			gameMakeMove(copy, arrayListGetAt(moves, i)->move);

			moveScore = -minimax(copy, depth - 1, -b, -a, -color, false);
			gameDestroy(copy);

			if (moveScore > bestValue)
			{
				bestValue = moveScore;
				bestMove = i;
			}

			// pruning
			a = max(a, moveScore);
			if (a >= b)
				break;
		}

		arrayListDestroy(moves);

		return (returnMove ? bestMove : bestValue);
	}
}

Move minimaxSuggestMove(ChessGame* game, int depth)
{
	int moveIndex = minimax(game, depth, -INF_SCORE, INF_SCORE, (game->currentPlayer == WHITE ? 1 : -1), true);
	MoveOptionsList* moves = gameGetAllValidMoves(game, game->currentPlayer, false);

	return arrayListGetAt(moves, moveIndex)->move;
}

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

int getGameScore(ChessGame* game)
{
	int score = 0;

	if (game->status == STATUS_DRAW)
	{
		return 0;
	}
	else if (game->status == STATUS_MATE)
	{
		score = -(game->currentPlayer == WHITE ? MATE_SCORE : -MATE_SCORE);
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				ChessPiece* piece = gameGetPieceAt(game, newPos(i, j));
				score += getPieceScore(piece);
			}
		}
	}

	return score;
	//return game->currentPlayer == WHITE ? score : -score;
}