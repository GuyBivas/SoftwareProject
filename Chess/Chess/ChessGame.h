#ifndef ChessGame_H_
#define ChessGame_H_
#include <stdbool.h>
#include "Enums.h"
#include "ArrayList.h"

//Definitions
#define _FIAR_GAME_PLAYER_1_SYMBOL 'X'
#define _FIAR_GAME_PLAYER_2_SYMBOL 'O'

#pragma region Structs

typedef struct chess_game {
	ChessPiece gameBoard[8][8];
	char currentPlayer;
	ArrayList* history;

} ChessGame;

typedef struct chess_piece {
	PIECE_TYPE type;
	COLOR color;
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

#pragma endregion

ChessGame* ChessGameCreate(int historySize);

ChessGame* ChessGameCopy(ChessGame* src);

void ChessGameDestroy(ChessGame* src);

CHESS_GAME_MESSAGE ChessGameSetMove(ChessGame* src, Move move);

bool ChessGameIsValidMove(ChessGame* src, Move move);

CHESS_GAME_MESSAGE ChessGameUndoPrevMove(ChessGame* src);

CHESS_GAME_MESSAGE ChessGamePrintBoard(ChessGame* src);

COLOR ChessGameGetCurrentPlayer(ChessGame* src);

GAME_STATUS CheckGameStatus(ChessGame* src);

#endif
