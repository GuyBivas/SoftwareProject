#pragma once
#ifndef MAINAUX_H
#define MAINAUX_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include "Enums.h"
#include "ChessGameManager.h"


ParsedCommand getCommand();

void commandExecution(ChessGameManager* manager, ParsedCommand command);
void exitGame(ChessGameManager* manager, bool isMallocError);
GAME_STATUS printWinner(ChessGameManager* manager);
int makeUserTurn(ChessGameManager* manager);
void makeComputerTurn(ChessGameManager* manager);

void ExecutionCommandGameMode(ChessGameManager* manager, ParsedCommand command);
void ExecutionCommandDifficulty(ChessGameManager* manager, ParsedCommand command);
void ExecutionCommandUserColor(ChessGameManager* manager, ParsedCommand command);

#endif