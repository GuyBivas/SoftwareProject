#ifndef ChessGame_H_
#define ChessGame_H_
#include <stdbool.h>
#include "Enums.h"
#include "MoveOptionsList.h"

//Definitions
#define _FIAR_GAME_PLAYER_1_SYMBOL 'X'
#define _FIAR_GAME_PLAYER_2_SYMBOL 'O'

#pragma region Structs

typedef struct chess_game {
	ChessPiece* gameBoard[8][8];
	PLAYER_COLOR currentPlayer;
	MoveOptionsList* history;
	GAME_STATUS status;
} ChessGame;

typedef struct chess_piece {
	PIECE_TYPE type;
	PLAYER_COLOR color;
	Position position;
} ChessPiece;

typedef struct position {
	int x;
	int y;
} Position;

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

ChessGame* gameCreate(int historySize);

ChessGame* gameCopy(ChessGame* src);

void gameDestroy(ChessGame* src);

MoveOptionsList gameGetMoves(ChessGame* src, Position pos);

CHESS_GAME_MESSAGE gameMakeMove(ChessGame* src, Move move);

bool gameIsValidMove(ChessGame* src, Move move);

CHESS_GAME_MESSAGE gameUndoPrevMove(ChessGame* src);

CHESS_GAME_MESSAGE gamePrintBoard(ChessGame* src); // Move to console mode?

void gameUpdateStatus(ChessGame* src);

#endif