#ifndef CHESSLOGIC_H
#define CHESSLOGIC_H

#include <stdbool.h>
#include "Enums.h"
#include "HistoryCircularArray.h"

#pragma region Structs

typedef struct position {
	int x;
	int y;
} Position;

typedef struct chess_piece {
	PIECE_TYPE type;
	PLAYER_COLOR color;
	Position position;
} ChessPiece;

typedef struct chess_game {
	ChessPiece* gameBoard[8][8];
	PLAYER_COLOR currentPlayer;
	HistoryCircularArray* history;
	GAME_STATUS status;
	ChessPiece* whiteKing;
	ChessPiece* blackKing;
} ChessGame;

typedef struct move {
	Position from;
	Position to;
} Move;

typedef struct moveOption {
	Position pos;
	bool isThreatened;
	bool isCapturing;
} moveOption;

#pragma endregion

bool logicIsValidMove(ChessGame* src, Move move);
bool logicCheckThreatened(ChessGame* src, Position pos, PLAYER_COLOR currColor);
void logicUpdateGameStatus(ChessGame* src);

#endif