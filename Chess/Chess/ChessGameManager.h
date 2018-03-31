#ifndef CHESSGAMEMANAGER_H
#define CHESSGAMEMANAGER_H

#include <stdbool.h>
#include "Enums.h"
#include "ChessGame.h"
#include "MoveOptionsList.h"
#include "HistoryCircularArray.h"

typedef struct chess_game_manager {
	ChessGame* game;
	HistoryCircularArray* history;
	PLAYER_MODE mode;
	GAME_DIFFICULTY difficulty;
} ChessGameManager;

ChessGameManager* createGameManager(int historySize);
ChessGameManager* gameCopy(ChessGameManager* src);
void gameDestroy(ChessGameManager* src);
MoveOptionsList* gameGetValidMoves(ChessGameManager* src, Position pos);
bool gameMakeMove(ChessGameManager* src, Move move);
CHESS_GAME_MESSAGE gameUndoPrevMove(ChessGameManager* src);
CHESS_GAME_MESSAGE gamePrintBoard(ChessGameManager* src); // Move to console mode?
ChessPiece* gameGetPieceAt(ChessGameManager* src, Position pos);
void gameSetPieceAt(ChessGameManager* src, Position pos, ChessPiece* newPiece);
void gameMovePiece(ChessGameManager* src, Move move);

IS_VALID_MOVE_RESULT logicIsValidMove(ChessGameManager* src, Move move);
bool logicCheckThreatened(ChessGameManager* src, Position pos, PLAYER_COLOR currColor);
void logicUpdateGameStatus(ChessGameManager* src);

#endif