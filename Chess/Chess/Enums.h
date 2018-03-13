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










#pragma endregion



// Type used for returning error codes from game functions
typedef enum chess_game_message_t {
	CHESS_GAME_INVALID_MOVE,
	CHESS_GAME_INVALID_ARGUMENT,
	CHESS_GAME_NO_HISTORY,
	CHESS_GAME_SUCCESS,
} CHESS_GAME_MESSAGE;