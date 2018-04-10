#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <stdlib.h>
#include <stdbool.h>
#include "Enums.h"
#include "Position.h"
#include "MoveOptionsList.h"

#define MAX_PIECE_MOVES 29  // 8*4-3 = 29 is the maximum possible moves for a queen
#define MAX_PLAYER_MOVES 137  // 8*4-3 + 8*8-4 + 8 + 8 + 8*4 = 137 is the maximum possible moves in a turn

typedef struct chess_piece {
	PIECE_TYPE type;
	PLAYER_COLOR color;
	Position position;
} ChessPiece;
	
typedef struct chess_game {
	ChessPiece* gameBoard[8][8];
	PLAYER_COLOR currentPlayer;
	GAME_STATUS status;
} ChessGame;

ChessPiece* gameGetPieceAt(ChessGame* game, Position pos);
void gameSetPieceAt(ChessGame* game, Position pos, ChessPiece* newPiece);
void gameMovePiece(ChessGame* game, Move move);
void gameMakeMove(ChessGame* src, Move move);

ChessGame* gameCreate();
ChessPiece* chessPieceCopy(ChessPiece* game);
ChessGame* gameCopy(ChessGame* game);
void gameDestroy(ChessGame* game);
MoveOptionsList* gameGetPieceValidMoves(ChessGame* game, Position pos, bool calcThreatened, bool returnAfterFirst);
MoveOptionsList* gameGetAllValidMoves(ChessGame* game, PLAYER_COLOR color, bool returnAfterFirst);
PLAYER_COLOR getOpositeColor(PLAYER_COLOR color);

IS_VALID_MOVE_RESULT logicIsValidMoveBasic(ChessGame* game, Move move);
IS_VALID_MOVE_RESULT logicIsValidMove(ChessGame* game, Move move);
bool logicCheckThreatened(ChessGame* game, Position pos, PLAYER_COLOR currColor);
void logicUpdateGameStatus(ChessGame* game);

#endif