#pragma once
#ifndef MAINAUX_H
#define MAINAUX_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include "Enums.h"
#include "ChessGame.h"


ParsedCommand getCommand();

void commandExecution(ChessGame* game, ParsedCommand command);
void exitGame(ChessGame* game, bool isMallocError);
GAME_STATUS printWinner(ChessGame* src);
int makeUserTurn(ChessGame* game);
void makeComputerTurn(ChessGame* game);

void ExecutionCommandGameMode(ChessGame* game, ParsedCommand command);
void ExecutionCommandDifficulty(ChessGame* game, ParsedCommand command);
void ExecutionCommandUserColor(ChessGame* game, ParsedCommand command);

#endif