#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <stdlib.h>
#include <stdbool.h>
#include "Enums.h"
#include "Position.h"
#include "MoveOptionsList.h"

typedef struct chess_piece {
	PIECE_TYPE type;
	PLAYER_COLOR color;
	Position position;
} ChessPiece;

typedef struct chess_game {
	ChessPiece* gameBoard[8][8];
	PLAYER_COLOR currentPlayer;
	GAME_STATUS status;
	ChessPiece* whiteKing;
	ChessPiece* blackKing;
} ChessGame;

ChessPiece* gameGetPieceAt(ChessGame* game, Position pos);
void gameSetPieceAt(ChessGame* game, Position pos, ChessPiece* newPiece);
void gameMovePiece(ChessGame* game, Move move);

ChessGame* gameCopy(ChessGame* game);
MoveOptionsList* gameGetValidMoves(ChessGame* game, Position pos);
PLAYER_COLOR getOpositeColor(PLAYER_COLOR color);

IS_VALID_MOVE_RESULT logicIsValidMoveBasic(ChessGame* game, Move move);
IS_VALID_MOVE_RESULT logicIsValidMove(ChessGame* game, Move move);
bool logicCheckThreatened(ChessGame* game, Position pos, PLAYER_COLOR currColor);
void logicUpdateGameStatus(ChessGame* game);

#endif