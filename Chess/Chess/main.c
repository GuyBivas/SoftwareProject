#include <stdio.h>
#include "ChessGameManager.h"
#include "MainAux.h"

#include <stdio.h>
#include "ChessGameManager.h"
#include "MainAux.h"

#define HISTORY_SIZE 6
bool mallocError;



int main()
{
	bool isSettingsState = true;
	ChessGameManager* manager = gameManagerCreate(HISTORY_SIZE);

	if (manager == NULL)
		exitGame(manager, true);

	printf("Specify game settings or type 'start' to begin a game with the current settings:\n");

	while (true)
	{
		if (isSettingsState)//settings mode
			isSettingsState = ExecutionSettingsState(manager);
		else if (manager->mode == TWO_PLAYERS || manager->game->currentPlayer == getOpositeColor(manager->computerColor))//TWO_PLAYERS mode oruser turn on ONE_PLAYER mode
			isSettingsState = makeUserTurn(manager);
		//	else // computer's turn 
		//	makeComputerTurn(manager)
	}
}

