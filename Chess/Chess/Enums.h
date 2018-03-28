#ifndef ENUMS_H
#define ENUMS_H

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
} PLAYER_COLOR;

typedef enum {
	STATUS_CHECK,
	STATUS_MATE,
	STATUS_DRAW,
	STATUS_NORMAL
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
	COMMAND_GAME_MODE,
	COMMAND_DIFFICULTY,
	COMMAND_USER_COLOR,
	COMMAND_LOAD,
	COMMAND_DEFAULT,
	COMMAND_PRINT_SETTINGS,
	COMMAND_START,
	//game state:
	COMMAND_MOVE,
	COMMAND_GET_MOVES,
	COMMAND_SAVE,
	COMMAND_UNDO,
	COMMAND_RESET,
	COMMAND_QUIT,
	COMMAND_INVALID_LINE

} COMMAND;

#pragma endregion

// Type used for returning error codes from game functions
typedef enum chess_game_message_t {
	CHESS_GAME_INVALID_MOVE,
	CHESS_GAME_INVALID_ARGUMENT,
	CHESS_GAME_NO_HISTORY,
	CHESS_GAME_SUCCESS,
} CHESS_GAME_MESSAGE;

/**
* A type used for errors
*/
typedef enum array_list_message_t {
	ARRAY_LIST_SUCCESS,
	ARRAY_LIST_INVALID_ARGUMENT,
	ARRAY_LIST_FULL,
	ARRAY_LIST_EMPTY
} ARRAY_LIST_MESSAGE;

#endif