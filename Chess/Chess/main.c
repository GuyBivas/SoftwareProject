#include <stdio.h>
#include "ChessGame.h"
#include "MainAux.h"




#define HISTORY_SIZE 6


int main()
{
	ChessGame* game = NULL;
	ParsedCommand command;
	PLAYER_COLOR playerColor;


	command.cmd = COMMAND_INVALID_LINE;//compiler wants initialization..

	while (game == NULL)
	{
		game = gameCreate(HISTORY_SIZE);
		if (game == NULL)
			exitGame(game, true);
	}


	while (command.cmd != COMMAND_START)//settings mode
	{


		command = getCommand();
		commandExecution(game, command);
		if (game->mode == ONE_PLAYER && command.cmd == COMMAND_USER_COLOR)
			playerColor = atoi(command.arg);
	}


	//while (true)//game mode
	//{
	//	if (game->status != STATUS_NORMAL)//MATE or DRAW
	//	{
	//		gamePrintBoard(game);
	//		printWinner(game);
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

