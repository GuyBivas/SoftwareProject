#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <stdbool.h>
#include "Enums.h"
#include "MoveOptionsList.h"
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
	PLAYER_MODE mode;
	GAME_DIFFICULTY difficulty;
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

ChessGame* gameCreate(int historySize);
ChessGame* gameCopy(ChessGame* src);
void gameDestroy(ChessGame* src);
MoveOptionsList* gameGetValidMoves(ChessGame* src, Position pos);
bool gameMakeMove(ChessGame* src, Move move);
CHESS_GAME_MESSAGE gameUndoPrevMove(ChessGame* src);
CHESS_GAME_MESSAGE gamePrintBoard(ChessGame* src); // Move to console mode?
ChessPiece* gameGetPieceAt(ChessGame* src, Position pos);
void gameSetPieceAt(ChessGame* src, Position pos, ChessPiece* newPiece);
void gameMovePiece(ChessGame* src, Move move);

IS_VALID_MOVE_RESULT logicIsValidMove(ChessGame* src, Move move);
bool logicCheckThreatened(ChessGame* src, Position pos, PLAYER_COLOR currColor);
void logicUpdateGameStatus(ChessGame* src);

Move newMove(Position from, Position to);
Position newPos(int x, int y);
bool posEqual(Position pos1, Position pos2);
bool gameIsInBoard(Position pos);
#endif