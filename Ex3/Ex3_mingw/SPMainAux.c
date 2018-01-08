#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SPMainAux.h"

char* my_strdup_main(const char* s)
{
	size_t size = strlen(s) + 1;
	char* p = malloc(size);
	if (p)
		memcpy(p, s, size);
	else
		printf("Error: malloc has failed\n");

	return p;
}

SPCommand getCommand()
{
	SPCommand cmd;
	char command[SP_MAX_LINE_LENGTH];
	fgets(command, SP_MAX_LINE_LENGTH, stdin);
	cmd = spParserPraseLine(command);

	return cmd;
}

int getMaxDepth()
{
	int curNum = 0;
	char usr_in[SP_MAX_LINE_LENGTH];
	const char delimiter[5] = " \t\r\n";
	const char *const_pointer = usr_in;
	char *tok, *next_tok;

	printf("Please enter the difficulty level between [1-7]:\n");
	fgets(usr_in, SP_MAX_LINE_LENGTH, stdin);
	char* strCopy = my_strdup_main(const_pointer);
	if (strCopy == NULL)
		return -2;

	tok = strtok(strCopy, delimiter);
	if (tok == NULL)
	{
		printf("Error: invalid level (should be between 1 to 7)\n");
		free(strCopy);
		return getMaxDepth();
	}

	next_tok = strtok(NULL, delimiter);
	if (strcmp(tok, "quit") == 0 && next_tok == NULL)
	{
		free(strCopy);
		return -1;
	}

	const_pointer = tok;
	if (next_tok != NULL || !spParserIsInt(const_pointer)) {
		printf("Error: invalid level (should be between 1 to 7)\n");
		free(strCopy);
		return getMaxDepth();
	}

	curNum = atoi(const_pointer);
	if (curNum < 1 || curNum > 7)
	{
		printf("Error: invalid level (should be between 1 to 7)\n");
		free(strCopy);
		return getMaxDepth();
	}

	free(strCopy);
	return curNum;
}

void makeComputerTurn(SPFiarGame* game, int depth)
{
	int col = spMinimaxSuggestMove(game, depth);

	if (col == -1)
		exitGame(game, true);

	spFiarGameSetMove(game, col);
	printf("Computer move: add disc to column %d\n", col + 1);
}

int makeUserTurn(SPFiarGame* game, int depth)
{
	int suggestedMove = 0;
	SP_FIAR_GAME_MESSAGE gameMsg;
	SPCommand curCommand;
	char command[SP_MAX_LINE_LENGTH];
	fgets(command, SP_MAX_LINE_LENGTH, stdin);

	curCommand = spParserPraseLine(command);

	if (curCommand.cmd == SP_INVALID_LINE)
	{
		printf("Error: invalid command\n");
		return makeUserTurn(game, depth);
	}
	else if (curCommand.cmd == SP_RESTART)
	{
		spFiarGameDestroy(game);
		return -1;
	}
	else if (curCommand.cmd == SP_QUIT)
	{
		exitGame(game, false);
	}
	else if (curCommand.cmd == SP_UNDO_MOVE)
	{
		if (spFiarGameUndoPrevMove(game) == SP_FIAR_GAME_NO_HISTORY)
			return makeUserTurn(game, depth);
	
		spFiarGameUndoPrevMove(game);
		return 0;
	}
	else if (curCommand.cmd == SP_ADD_DISC)
	{
		if (!curCommand.validArg)
		{
			printf("Error: column number must be in range 1-7\n");
			return makeUserTurn(game, depth);
		}

		gameMsg = spFiarGameSetMove(game, curCommand.arg - 1);
		if (gameMsg == SP_FIAR_GAME_INVALID_ARGUMENT)
		{
			printf("Error: column number must be in range 1-7\n");
			return makeUserTurn(game, depth);
		}

		if (gameMsg == SP_FIAR_GAME_INVALID_MOVE)
		{
			printf("Error: column %d is full\n", curCommand.arg);
			return makeUserTurn(game, depth);
		}

		if (gameMsg == SP_FIAR_GAME_SUCCESS)
			return 0;
	}
	else if (curCommand.cmd == SP_SUGGEST_MOVE)
	{
		suggestedMove = spMinimaxSuggestMove(game, depth);

		if (suggestedMove == -1)
			exitGame(game, true);

		printf("Suggested move: drop a disc to column %d\n", suggestedMove + 1);
		return makeUserTurn(game, depth);
	}

	return 0; // shouldnt get here
}

void printWinner(char winner)
{
	if (winner == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		printf("Game over: you win\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");

	if (winner == SP_FIAR_GAME_PLAYER_2_SYMBOL)
		printf("Game over: computer wins\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");

	if (winner == SP_FIAR_GAME_TIE_SYMBOL)
		printf("Game over: it's a tie\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");
}

void exitGame(SPFiarGame* game, bool isMallocError)
{
	spFiarGameDestroy(game);

	if (!isMallocError)
		printf("Exiting...\n");

	exit(0);
}