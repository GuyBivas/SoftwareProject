#include "ChessGameManager.h"

ChessGameManager* gameManagerCreate(int historySize)
{
	if (historySize <= 0) //TODO: check if needed
		return NULL;

	ChessGameManager* manager = (ChessGameManager*)malloc(sizeof(ChessGameManager));

	if (manager == NULL)
	{
		mallocError = true;
		return NULL;
	}

	gameManagerSetDefault(manager);
	manager->history = circularArrayCreate(historySize);

	if (manager->history == NULL)
	{
		free(manager);
		mallocError = true;
		return NULL;
	}

	manager->game = gameCreate();

	return manager;
}

void gameManagerDestroy(ChessGameManager* src)
{
	if (src == NULL)
		return;

	circularArrayDestroy(src->history);
	gameDestroy(src->game);
	free(src);
}

void gameManagerMakeMove(ChessGameManager* src, Move move)
{
	circularArrayAdd(src->history, gameCopy(src->game));
	gameMakeMove(src->game, move);
}

void gameManagerUndoPrevMove(ChessGameManager* src)
{
	ChessGame* hist = circularArrayGetCurrent(src->history);

	src->game->currentPlayer = hist->currentPlayer;
	src->game->status = hist->status;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			src->game->gameBoard[i][j] = hist->gameBoard[i][j];
		}
	}

	circularArrayRemove(src->history);
}

char printPiece(ChessPiece* piece)
{
	char c = ' ';

	if (piece == NULL)
	{
		c='_';
		return c;
	}

	switch (piece->type)
	{
	case PAWN:
		c = 'M';
		break;
	case KNIGHT:
		c = 'N';
		break;
	case ROOK:
		c = 'R';
		break;
	case BISHOP:
		c = 'B';
		break;
	case QUEEN:
		c = 'Q';
		break;
	case KING:
		c = 'K';
		break;
	}

	if (piece->color == WHITE)
		c += 'a' - 'A';

	return c;
}

CHESS_GAME_MESSAGE gameManagerPrintBoard(ChessGameManager* src)
{
	if (src == NULL)
		return CHESS_GAME_INVALID_ARGUMENT;

	for (int j = 7; j >= 0; j--)
	{
		printf("%d| ", j + 1);
		for (int i = 0; i < 8; i++)
		{
			printf("%c ", printPiece(gameGetPieceAt(src->game, newPos(j, i))));
		}

		printf("|\n");
	}

	printf("  -----------------\n");
	printf("   A B C D E F G H\n");

	return CHESS_GAME_SUCCESS;
}

void gameManagerSetDefault(ChessGameManager* manager)
{
	manager->mode = ONE_PLAYER;
	manager->difficulty = EASY;
	manager->computerColor = BLACK;
}