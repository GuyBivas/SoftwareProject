#include <stdio.h>
#include "ChessGameManager.h"
#include "MainAux.h"
//#include "SDL_Shared.h"

int main(int argc, char *argv[])
{
	if (argc > 10)
		printf("%s", argv[0]);

	bool isSettingsState = true;
	ChessGameManager* manager = gameManagerCreate(HISTORY_SIZE);

	if (manager == NULL)
		exitGame(manager, true);

	printf("Specify game settings or type 'start' to begin a game with the current settings:\n");

	while (true)
	{
		if (isSettingsState) //settings mode
			isSettingsState = ExecutionSettingsState(manager);
		else if (manager->mode == TWO_PLAYERS || manager->game->currentPlayer == getOpositeColor(manager->computerColor))//TWO_PLAYERS mode or user turn on ONE_PLAYER mode
			isSettingsState = makeUserTurn(manager);
		else // computer's turn 
			makeComputerTurn(manager);
	}
	return 0;
}