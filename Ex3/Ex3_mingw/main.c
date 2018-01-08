#include "SPMainAux.h"

int main()
{
	SPFiarGame* game = NULL;
	SPCommand command;
	int depth = 0;

	while (true)
	{
		game = spFiarGameCreate(HISTORY_SIZE);
		if (game == NULL)
			exitGame(game, true);

		depth = getMaxDepth();

		if (depth == -2)
			exitGame(game, true);
		if (depth == -1)
			exitGame(game, false);		

		while (true)
		{
			if (spFiarCheckWinner(game) != '\0')
			{
				spFiarGamePrintBoard(game);
				printWinner(spFiarCheckWinner(game));
				command = getCommand();

				while (command.cmd != SP_QUIT && command.cmd != SP_UNDO_MOVE && command.cmd != SP_RESTART)
				{
					if (command.cmd == SP_INVALID_LINE)
						printf("Error: invalid command\n");
					else
						printf("Error: the game is over\n");

					command = getCommand();
				}

				if (command.cmd == SP_QUIT)
				{
					exitGame(game, false);
				}
				else if (command.cmd == SP_RESTART)
				{
					spFiarGameDestroy(game);
					break;
				}
				else
				{
					if (spFiarCheckWinner(game) != SP_FIAR_GAME_PLAYER_1_SYMBOL)
						spFiarGameUndoPrevMove(game);

					spFiarGameUndoPrevMove(game);
					continue;
				}
			}

			if (spFiarGameGetCurrentPlayer(game) == SP_FIAR_GAME_PLAYER_2_SYMBOL)
			{
				makeComputerTurn(game, depth);
				continue;
			}
			else if (spFiarGameGetCurrentPlayer(game) == SP_FIAR_GAME_PLAYER_1_SYMBOL)
			{
				spFiarGamePrintBoard(game);
				printf("Please make the next move:\n");

				if (makeUserTurn(game, depth) == -1)
					break;
				else
					continue;
			}
		}

		printf("Game restarted!\n");
	}
}