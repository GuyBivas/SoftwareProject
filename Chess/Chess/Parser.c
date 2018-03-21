#include <stdio.h>
#include "Parser.h"
#include "Enums.h"
#include <stdlib.h>
#include <string.h>

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
	case GAME_MODE:
		return "game_mode";
	case DIFFICULTY:
		return "difficulty";
	case USER_COLOR:
		return "user_color";
	case LOAD:
		return "load";
	case DEFAULT:
		return "default";
	case QUIT:
		return "quit";
	case START:
		return "start";
		// game commands: 
	case MOVE:
		return "move";
	case GET_MOVES:
		return "get_moves";
	case SAVE:
		return "save";
	case UNDO:
		return "undo";
	case RESET:
		return "reset";

	default:
		return "notcmd";
	}
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

	output.cmd = INVALID_LINE;
	strcpy(copy, str);
	word = strtok(copy, " \t\r\n");

	for (int command = 0; command < 14; command++)
	{
		if (word != NULL && strcmp(word, enumToStringSC(command)) == 0) //checks if the input contains valid command
			output.cmd = command;
	}

	if (output.cmd == GAME_MODE)
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
	if (output.cmd == DIFFICULTY)
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
	if (output.cmd == USER_COLOR)
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
	if (output.cmd == LOAD)//only checks that the path string is not NULL
	{
		word = strtok(NULL, " \t\r\n");
		if (word != NULL )
		{
			output.arg = word;
			output.validArg = true;
		}
		else
			output.validArg = false;
	}
	if (output.cmd == SAVE)
	{
		word = strtok(NULL, " \t\r\n");
		if (word == NULL)
			output.validArg = false;
		else
		{
			output.arg = word;
			output.validArg = true;
		}

	}
	if (output.cmd == MOVE)
	{
		for (int i = 0; i < 3; i++)
		{
			word = strtok(NULL, " \t\r\n");
			if (word != NULL)
			{
				if (i != 1)
				{
					if (isValidLocation(word)==false)
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
	if (output.cmd == GET_MOVES)
	{
		word = strtok(NULL, " \t\r\n");
		if (word == NULL)
			output.validArg = false;
		if (isValidLocation(word) == false)
			output.validArg = false;
	}
	
	return output;
}


bool FileExists(const char *path)
{
	FILE *fp;
	fpos_t fsize = 0;

	if (!fopen_s(&fp, path, "r"))
	{
		fseek(fp, 0, SEEK_END);
		fgetpos(fp, &fsize);
		fclose(fp);
	}

	return fsize > 0;
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
