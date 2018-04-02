#include <stdio.h>
#include "ChessGameManager.h"
#include "MainAux.h"

#define HISTORY_SIZE 6

int main()
{
	ChessGameManager* game = NULL;
	ParsedCommand command;
	PLAYER_COLOR playerColor;
	bool isSettingsState = true;


	command.cmd = COMMAND_INVALID_LINE;//compiler wants initialization..

	while (game == NULL)
	{
		game = gameManagerCreate(HISTORY_SIZE);
		if (game == NULL)
			exitGame(game, true);
	}


	while (command.cmd != COMMAND_START)//settings mode
	{


		command = getCommand();
	//	commandExecution(game, command);
		if (game->mode == ONE_PLAYER && command.cmd == COMMAND_USER_COLOR)
			playerColor = atoi(command.arg);
	}


	//while (true)//game mode
	//{
	//	if (game->status != STATUS_NORMAL)//MATE or DRAW
	//	{
	//		gameManagerPrintBoard(game);
		printWinner(game);
	//		exitGame(game, false);
	//	}
	//	else if (game->mode == ONE_PLAYER)
	//	{
	//		if (game->currentPlayer == playerColor)
	//			makeUserTurn(game);
	//		else
	//			makeComputerTurn(game);
	//	}
	//	else
	//	{
	//	//	makeUserTurn(game);
	//	}

	}

