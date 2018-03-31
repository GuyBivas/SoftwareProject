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

void commandExecution(ChessGameManager* game, ParsedCommand command);
void exitGame(ChessGameManager* game, bool isMallocError);
GAME_STATUS printWinner(ChessGameManager* src);
int makeUserTurn(ChessGameManager* game);
void makeComputerTurn(ChessGameManager* game);

void ExecutionCommandGameMode(ChessGameManager* game, ParsedCommand command);
void ExecutionCommandDifficulty(ChessGameManager* game, ParsedCommand command);
void ExecutionCommandUserColor(ChessGameManager* game, ParsedCommand command);

#endif