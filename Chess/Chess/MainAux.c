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


void commandExecution(ChessGame* game, ParsedCommand command)
{
	switch (command.cmd)
	{
	case COMMAND_GAME_MODE:
		ExecutionCommandGameMode(game, command);

	case COMMAND_DIFFICULTY:
		ExecutionCommandDifficulty(game, command);
	case COMMAND_USER_COLOR:
		ExecutionCommandUserColor(game, command);
	case COMMAND_LOAD:// continue tommorow from here
		return "load";
	case COMMAND_DEFAULT:
		game->mode = ONE_PLAYER;
		game->difficulty = EASY;
		game->currentPlayer = WHITE;
	case COMMAND_QUIT:
		return "quit";
	case COMMAND_START:
		return "start";
	}
}


void ExecutionCommandGameMode(ChessGame* game, ParsedCommand command)
{
	if (command.validArg == true)
	{
		if (strcmp(command.arg, "1") == 0)
			game->mode = ONE_PLAYER;
		else
			game->mode = TWO_PLAYERS;
		printf("Game mode is set to %s\n", enumArgToString(command));
	}
	else
	{
		printf("Wrong game mode\n");
	}
}


void ExecutionCommandDifficulty(ChessGame* game, ParsedCommand command)
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


void ExecutionCommandUserColor(ChessGame* game, ParsedCommand command)
{
	if (game->mode == ONE_PLAYER)
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


void exitGame(ChessGame* game, bool isMallocError)
{
	gameDestroy(game);

	if (!isMallocError)
		printf("Exiting...\n");

	exit(0);
}


GAME_STATUS printWinner(ChessGame* src)
{
	if (src->status == STATUS_MATE && src->currentPlayer == WHITE)
		printf("Checkmate! white player wins the game\n");

	if (src->status == STATUS_MATE && src->currentPlayer == WHITE)
		printf("Checkmate! black player wins the game\n");

	if (src->status == STATUS_DRAW)
		printf("The game ends in a draw\n");
}


int makeUserTurn(ChessGame* game)
{
}


void makeComputerTurn(ChessGame* game)
{

}


