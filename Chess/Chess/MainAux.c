#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "MainAux.h"

ParsedCommand getCommand()
{
	ParsedCommand cmd;
	char command[MAX_LINE_LENGTH];
	fgets(command, MAX_LINE_LENGTH, stdin);
	cmd = ParserPraseLine(command);

	return cmd;
}


void commandExecution(ChessGameManager* manager, ParsedCommand command)
{
	switch (command.cmd)
	{
	case COMMAND_GAME_MODE:
		ExecutionCommandGameMode(manager, command);

	case COMMAND_DIFFICULTY:
		ExecutionCommandDifficulty(manager, command);
	case COMMAND_USER_COLOR:
		ExecutionCommandUserColor(manager, command);
	case COMMAND_LOAD:// continue tommorow from here
		return; // "load";
	case COMMAND_DEFAULT:
		manager->mode = ONE_PLAYER;
		manager->difficulty = EASY;
		manager->game->currentPlayer = WHITE;
	case COMMAND_QUIT:
		return; // "quit";
	case COMMAND_START:
		return; // "start";
	}
}


void ExecutionCommandGameMode(ChessGameManager* manager, ParsedCommand command)
{
	if (command.validArg == true)
	{
		if (strcmp(command.arg, "1") == 0)
			manager->mode = ONE_PLAYER;
		else
			manager->mode = TWO_PLAYERS;
		printf("Game mode is set to %s\n", enumArgToString(command));
	}
	else
	{
		printf("Wrong game mode\n");
	}
}


void ExecutionCommandDifficulty(ChessGameManager* game, ParsedCommand command)
{
	if (command.validArg == true)
	{
		game->difficulty = atoi(command.arg) + 1;
		printf("Difficulty level is set to %s\n", enumArgToString(command));
	}
	else
	{
		printf("Wrong difficulty level. The value should be between 1 to 5\n");
	}
}


void ExecutionCommandUserColor(ChessGameManager* manager, ParsedCommand command)
{
	if (manager->mode == ONE_PLAYER)
	{
		if (command.validArg == true)
			printf("User color is set to %s\n", enumArgToString(command));
		else
			printf("Wrong user color. The value should be 0 or 1\n");
	}
	else
	{
		printf("ERROR: invalid command\n");
	}
}


void exitGame(ChessGameManager* manager, bool isMallocError)
{
	gameDestroy(manager);

	if (!isMallocError)
		printf("Exiting...\n");

	exit(0);
}


GAME_STATUS printWinner(ChessGameManager* manager)
{
	if (manager->game->status == STATUS_MATE && manager->game->currentPlayer == WHITE)
		printf("Checkmate! white player wins the game\n");

	if (manager->game->status == STATUS_MATE && manager->game->currentPlayer == WHITE)
		printf("Checkmate! black player wins the game\n");

	if (manager->game->status == STATUS_DRAW)
		printf("The game ends in a draw\n");

	return 0;
}


int makeUserTurn(ChessGameManager* manager)
{
	return 0;
}


void makeComputerTurn(ChessGameManager* manager)
{

}


