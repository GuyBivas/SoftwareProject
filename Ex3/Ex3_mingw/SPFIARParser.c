#include <stdio.h>
#include "SPFIARParser.h"
#include <stdlib.h>
#include <string.h>

char* enumToString(SP_COMMAND c)
{
	switch (c)
	{
	case SP_UNDO_MOVE:
		return "SP_UNDO_MOVE";
	case SP_ADD_DISC:
		return "SP_ADD_DISC";
	case SP_SUGGEST_MOVE:
		return "SP_SUGGEST_MOVE";
	case SP_QUIT:
		return "SP_QUIT";
	case SP_RESTART:
		return "SP_RESTART";
	case SP_INVALID_LINE:
		return "SP_INVALID_LINE";

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

SPCommand spParserPraseLine(const char* str)
{
	SPCommand output;
	char* word;
	char copy[1024];

	output.cmd = SP_INVALID_LINE;
	strcpy(copy, str);
	word = strtok(copy, " \t\r\n");

	for (int command = 0; command < 6; command++)
	{
		if (word != NULL && strcmp(word, enumToString(command)) == 0) //checks if the input contains valid command
			output.cmd = command;
	}

	if (output.cmd == SP_ADD_DISC) //in case its the SP_ADD_DISC command
	{
		word = strtok(NULL, " \t\r\n");
		if (word != NULL && spParserIsInt(word))
		{
			output.arg = atoi(word);
		}
		
		output.validArg = !(output.arg < 1 || output.arg > 7 || strtok(NULL, " \t\r\n") != NULL);
	}

	if (strtok(NULL, " \t\r\n") != NULL) //in case there are unnecessary letters
	{
		output.cmd = SP_INVALID_LINE;
		output.validArg = false;
	}

	return output;
}