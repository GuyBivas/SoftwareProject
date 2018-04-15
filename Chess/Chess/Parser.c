#include "Parser.h"

/**
* Given an SP_COMMAND, this function returns the relevant string.
*
* @param c - the current SP_COMMAND
* @return
* a string suiting the recieved SP_COMMAND
*/
char* enumToStringSC(COMMAND c)
{
	switch (c)
	{
		//Settings command
	case COMMAND_GAME_MODE:
		return "game_mode";
	case COMMAND_DIFFICULTY:
		return "difficulty";
	case COMMAND_USER_COLOR:
		return "user_color";
	case COMMAND_LOAD:
		return "load";
	case COMMAND_DEFAULT:
		return "default";
	case COMMAND_QUIT:
		return "quit";
	case COMMAND_START:
		return "start";
		// game commands: 
	case COMMAND_MOVE:
		return "move";
	case COMMAND_GET_MOVES:
		return "get_moves";
	case COMMAND_SAVE:
		return "save";
	case COMMAND_UNDO:
		return "undo";
	case COMMAND_RESET:
		return "reset";

	default:
		return "notcmd";
	}
}

char* enumArgToString(ParsedCommand c)
{
	switch (c.cmd)
	{
		//Settings command
	case COMMAND_GAME_MODE:
		if (strcmp(c.arg, "1") == 0)
			return "1-player";
		else
			return "2-player";

	case COMMAND_DIFFICULTY:
		return difficultyArgPrint(c.arg);
	case COMMAND_USER_COLOR:
		if (strcmp(c.arg, "0") == 0)
			return "black";
		else
			return "white";
	}
	return NULL;
}

char* difficultyArgPrint(char* c)
{
	switch (c[0])
	{
	case '1':
		return "amateur";
		break;
	case '2':
		return "easy";
		break;
	case '3':
		return "moderate";
		break;
	case '4':
		return "hard";
		break;
	case '5':
		return "expert";
		break;
	}
	return NULL;
}

GAME_DIFFICULTY difficultyStringToArg(char* c)
{
	if (strcmp(c, "DIFFICULTY: amateur\n") == 0)
		return AMATEUR;
	if (strcmp(c, "DIFFICULTY: easy\n") == 0)
		return EASY;
	if (strcmp(c, "DIFFICULTY: moderate\n") == 0)
		return MODERATE;
	if (strcmp(c, "DIFFICULTY: hard\n") == 0)
		return HARD;
	if (strcmp(c, "DIFFICULTY: expert\n") == 0)
		return EXPERT;
	return EASY;
}

bool spParserIsInt(const char* str)
{
	for (unsigned int i = 0; i < strlen(str); i++)
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return false;
	}

	return true;
}

ParsedCommand ParserPraseLine(const char* str)
{
	ParsedCommand output;
	char* word;
	char copy[2048];
	char* to = "to";

	output.cmd = COMMAND_INVALID_LINE;
	strcpy(copy, str);
	word = strtok(copy, " \t\r\n");

	for (int command = 0; command < 14; command++)
	{
		if (word != NULL && strcmp(word, enumToStringSC(command)) == 0) //checks if the input contains valid command
			output.cmd = command;
	}


	if (output.cmd == COMMAND_GAME_MODE)
	{
		word = strtok(NULL, " \t\r\n");
		if (word == NULL)
			output.validArg = false;
		else if (spParserIsInt(word))
		{
			if (atoi(word) == 1 || atoi(word) == 2)
			{
				output.arg = word;
				output.validArg = true;
			}
			else
				output.validArg = false;
		}
	}
	if (output.cmd == COMMAND_DIFFICULTY)
	{
		word = strtok(NULL, " \t\r\n");
		if (word == NULL)
			output.validArg = false;
		else if (spParserIsInt(word))
		{
			if (atoi(word) >= 1 || atoi(word) <= 5)
			{
				output.arg = word;
				output.validArg = true;
			}
			else
				output.validArg = false;
		}
	}
	if (output.cmd == COMMAND_USER_COLOR)
	{
		word = strtok(NULL, " \t\r\n");
		if (word == NULL)
			output.validArg = false;
		else if (spParserIsInt(word))
		{
			if (atoi(word) == 0 || atoi(word) == 1)
			{
				output.arg = word;
				output.validArg = true;
			}
			else
				output.validArg = false;
		}
	}
	if (output.cmd == COMMAND_LOAD)//only checks that the path string is not NULL
	{
		word = strtok(NULL, " \t\r\n");
		if (word != NULL)
		{
			output.arg = word;
			output.validArg = true;
		}
		else
			output.validArg = false;
	}
	if (output.cmd == COMMAND_SAVE)
	{
		word = strtok(NULL, " \t\r\n");
		if (word == NULL)
			output.validArg = false;
		else
		{
			output.arg = (char*)malloc(sizeof(char) * (sizeof(word) / sizeof(char))); // TODO: free
			if (output.arg == NULL)
			{
				mallocError = true;
				return output;
			}
			strcpy(output.arg, word);
			//output.arg = word;
			output.validArg = true;
		}

	}
	if (output.cmd == COMMAND_MOVE)
	{
		output.arg = (char*)malloc(sizeof(char) * 4); // TODO: free

		if (output.arg == NULL)
		{
			mallocError = true;
			return output;
		}

		for (int i = 0; i < 3; i++)
		{
			word = strtok(NULL, " \t\r\n");
			if (word != NULL)
			{
				if (i != 1)
				{
					if (isValidLocation(word) == false)
						output.validArg = false;
					else
					{
						output.arg[i] = word[1];
						output.arg[i + 1] = word[3];
						output.validArg = true;
					}
				}
				else if (!(strcmp(word, to)))
					output.validArg = false;
			}
		}

		//if (strtok(NULL, " \t\r\n") != NULL) //in case there are unnecessary letters
		//{
		//	output.cmd = SP_INVALID_LINE;
		//	output.validArg = false;
		//}

		//return output;
	}
	if (output.cmd == COMMAND_GET_MOVES)
	{
		word = strtok(NULL, " \t\r\n");
		if (word == NULL)
			output.validArg = false;
		if (isValidLocation(word) == false)
			output.validArg = false;
		output.arg = word;
	}

	return output;
}

bool isValidLocation(const char *word)
{
	if (!(word[0] == '<' && word[4] == '>'))
		return false;
	if (!(word[1] >= '1' && word[1] <= '8'))
		return false;
	if (word[2] != ',')
		return false;
	if (!(word[3] >= 'A' && word[3] <= 'H'))
		return false;

	return true;
}