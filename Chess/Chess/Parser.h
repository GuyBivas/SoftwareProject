#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include "Enums.h"
#include <stdlib.h>
#include <string.h>

//specify the maximum line length
#define MAX_LINE_LENGTH 2048

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	char* arg;
} ParsedCommand;

/**
* Checks if a specified string represents a valid integer. It is recommended
* to use this function prior to calling the standard library function atoi.
*
* @return
* true if the string represents a valid integer, and false otherwise.
*/
bool spParserIsInt(const char* str);

/**
* Parses a specified line. If the line is a command which has a valid
* argument then the argument is parsed and is saved in the field arg and the
* field validArg is set to true. In any other case then 'validArg' is set to
* false and the value 'arg' is undefined
*
* @return
* A parsed line such that:
*   cmd - contains the command type, if the line is invalid then this field is
*         set to INVALID_LINE
*   validArg - is set to true if the command is add_disc and the integer argument
*              is valid
*   arg      - the integer argument in case validArg is set to true
*/
ParsedCommand ParserPraseLine(const char* str);


bool isValidLocation(const char *word);
bool FileExists(const char *path);
char* difficultyArgPrint(char* c);
char* enumArgToString(ParsedCommand c);
GAME_DIFFICULTY difficultyStringToArg(char* c);

#endif