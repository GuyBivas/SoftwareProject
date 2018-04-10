#pragma once
#ifndef MAINAUX_H
#define MAINAUX_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include "Enums.h"
#include "ChessGameManager.h"

#define HISTORY_SIZE 6
bool mallocError;


ParsedCommand getCommand();

bool ExecutionSettingsState(ChessGameManager* manager);
void exitGame(ChessGameManager* manager, bool isMallocError);
void printWinner(ChessGameManager* manager);
bool makeUserTurn(ChessGameManager* manager);
void makeComputerTurn(ChessGameManager* manager);

void ExecutionCommandGameMode(ChessGameManager* manager, ParsedCommand command);
void ExecutionCommandDifficulty(ChessGameManager* manager, ParsedCommand command);
void ExecutionCommandUserColor(ChessGameManager* manager, ParsedCommand command);

#endif