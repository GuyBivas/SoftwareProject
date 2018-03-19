#ifndef ENUMS_H_
#define ENUMS_H_

#pragma region Game State

typedef enum {
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
} PIECE_TYPE;

typedef enum {
	BLACK,
	WHITE
} COLOR;

typedef enum {
	CHECK,
	MATE,
	DRAW,
	NORMAL
} GAME_STATUS;


#pragma endregion


#pragma region Settings State

typedef enum {
	AMATEUR,
	EASY,
	MODERATE,
	HARD,
	EXPERT
} GAME_DIFFICULTY;


typedef enum {
	ONE_PLAYER,
	TWO_PLAYERS
} PLAYER_MODE;






#pragma endregion


#pragma region Commands
//a type used to represent a command
typedef enum {
	// settings state:
	GAME_MODE,
	DIFFICULTY,
	USER_COLOR,
	LOAD,
	DEFAULT,
	PRINT_SETTINGS,
	START,
	//game state:
	MOVE,
	GET_MOVES,
	SAVE,
	UNDO,
	RESET,
	QUIT,
	INVALID_LINE

} COMMAND;

#pragma endregion


// Type used for returning error codes from game functions
typedef enum chess_game_message_t {
	CHESS_GAME_INVALID_MOVE,
	CHESS_GAME_INVALID_ARGUMENT,
	CHESS_GAME_NO_HISTORY,
	CHESS_GAME_SUCCESS,
} CHESS_GAME_MESSAGE;

#endif