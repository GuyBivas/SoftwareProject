#include "MainAux.h"

ParsedCommand getCommand()
{
	ParsedCommand cmd;
	char command[MAX_LINE_LENGTH];
	fgets(command, MAX_LINE_LENGTH, stdin);
	cmd = ParserPraseLine(command);

	return cmd;
}

bool isFileExists(const char *fname)
{
	FILE *file;
	if ((file = fopen(fname, "r")))
	{
		fclose(file);
		return true;
	}
	return false;
}

void removeSpaces(char* c)
{
	int count = 0;

	for (int i = 0; c[i]; i++)
		if (c[i] != ' ')
			c[count++] = c[i];
	c[count] = '\0';
}

bool ExecutionSettingsState(ChessGameManager* manager)
{
	ParsedCommand command = getCommand();

	switch (command.cmd)
	{
	case COMMAND_GAME_MODE:
		ExecutionCommandGameMode(manager, command);
		return true;
	case COMMAND_DIFFICULTY:
		ExecutionCommandDifficulty(manager, command);
		return true;
	case COMMAND_USER_COLOR:
		ExecutionCommandUserColor(manager, command);
		return true;
	case COMMAND_LOAD:
		if (command.validArg == false || !isFileExists(command.arg))
		{
			printf("Error: File doesn't exist or cannot be opened\n");
			return true;
		}
		loadFile(command.arg, manager);
		return true;
	case COMMAND_DEFAULT:
		gameManagerSetDefault(manager);
		return true;
	case COMMAND_PRINT_SETTINGS:
		printf("Error: File doesn't exist or cannot be opened\n");
	case COMMAND_QUIT:
		exitGame(manager, false);
		return true;
	case COMMAND_START:
		printf("Starting game...\n");
		return false; // "start";
	case COMMAND_INVALID_LINE:
		printf("ERROR: invalid command\n");
		return true;

	}


	return true;
}

void ExecutionCommandGameMode(ChessGameManager* manager, ParsedCommand command)
{
	if (command.validArg == true)
	{
		if (strcmp(command.arg, "1") == 0)
			manager->mode = ONE_PLAYER;
		else
			manager->mode = TWO_PLAYERS;
		printf("Game mode is set to %s\n", enumArgToString(command));
	}
	else
	{
		if (command.arg != NULL)
			free(command.arg);
		printf("Wrong game mode\n");
	}
}

void ExecutionCommandDifficulty(ChessGameManager* manager, ParsedCommand command)
{
	if (command.validArg == true)
	{
		manager->difficulty = atoi(command.arg) - 1;
		printf("Difficulty level is set to %s\n", enumArgToString(command));
	}
	else
	{
		if (command.arg != NULL)
			free(command.arg);
		printf("Wrong difficulty level. The value should be between 1 to 5\n");
	}
}

void ExecutionCommandUserColor(ChessGameManager* manager, ParsedCommand command)
{
	if (manager->mode == ONE_PLAYER)
	{
		if (command.validArg == true)
		{
			printf("User color is set to %s\n", enumArgToString(command));
		}
		else
		{
			if (command.arg != NULL)
				free(command.arg);
			printf("Wrong user color. The value should be 0 or 1\n");
		}
	}
	else
	{

		printf("ERROR: invalid command\n");
	}
}

Move getLastMove(ChessGameManager* manager)
{
	ChessGame* beforeLastMove = circularArrayGetCurrent(manager->history);
	PLAYER_COLOR currentPlayer = manager->game->currentPlayer;
	Move lastMove;

	currentPlayer = getOpositeColor(manager->game->currentPlayer);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position pos = newPos(i, j);
			ChessPiece* currentPiece = gameGetPieceAt(manager->game, pos);
			ChessPiece* previousPiece = gameGetPieceAt(beforeLastMove, pos);

			if (previousPiece != NULL && currentPiece == NULL)
				lastMove.from = pos;

			if (currentPiece != NULL && (previousPiece == NULL || currentPiece->color != previousPiece->color))
				lastMove.to = pos;
		}
	}

	return lastMove;
}

void printLastMove(ChessGameManager* manager, Move* lastMove)
{
	Position from = lastMove->from;
	Position to = lastMove->to;
	char color[6];
	PLAYER_COLOR currentPlayer;

	currentPlayer = getOpositeColor(manager->game->currentPlayer);

	if (currentPlayer == WHITE)
		strcpy(color, "white");
	else
		strcpy(color, "black");

	printf("Undo move for %s player: <%d,%d> -> <%d,%d>\n", color, from.x, from.y, to.x, to.y);
}

void ExecutionCommandUndo(ChessGameManager* manager)
{
	int k = 2;
	Move lastMove;

	if (circularArrayListIsEmpty(manager->history))
	{
		printf("Empty history, no move to undo\n");
		return;
	}
	else if (circularArrayListSize(manager->history) == 1 ||
		((manager->game->status == STATUS_MATE || manager->game->status == STATUS_DRAW) &&
		(manager->game->currentPlayer == manager->computerColor) && (manager->mode == ONE_PLAYER)))
	{
		k = 1;
	}

	for (int i = 0; i < k; i++)
	{
		lastMove = getLastMove(manager);
		printLastMove(manager, &lastMove);
		gameManagerUndoPrevMove(manager);
	}
}

void ExecutionCommandMove(ChessGameManager* manager, ParsedCommand command)
{
	if (command.validArg == false)
	{
		printf("Invalid position on the board\n");
		return;
	}

	Position from = newPos(command.arg[0] - '1', command.arg[1] - 'A');
	Position to = newPos(command.arg[2] - '1', command.arg[3] - 'A');
	Move move = newMove(from, to);

	switch (logicIsValidMove(manager->game, move))
	{
	case IVMR_INVALID_POSITION:
		printf("Invalid position on the board\n");
		return;
	case IVMR_NO_PIECE_IN_POS:
		printf("The specified position does not contain your piece\n");
		return;
	case IVMR_ILLEGAL_MOVE:
		printf("Illegal move\n");
		return;
	case IVMR_KING_STILL_THREATENED:
		printf("Illegal move: king is still threatened\n");
		return;
	case IVMR_KING_GET_THREATENED:
		printf("Ilegal move: king will be threatened\n");
		return;

	case IVMR_VALID:
		gameManagerMakeMove(manager, move);
		if (command.arg != NULL)
			free(command.arg);
	}

	if (manager->game->status == STATUS_MATE || manager->game->status == STATUS_DRAW)
	{
		printWinner(manager);
		exitGame(manager, true);
	}
}

void ExecutionGetMoves(ChessGameManager* manager, ParsedCommand command)//TODO
{
	int x = command.arg[1] - '1';
	int y = command.arg[3] - 'A';
	Position pos = newPos(x, y);

	if (command.validArg == false)
	{
		printf("Invalid position on the board\n");
		return;
	}
	if (gameGetPieceAt(manager->game, pos) == NULL)
	{
		printf("The specified position does not contain a player piece\n");
		return;
	}

	MoveOptionsList* validMoves = gameGetPieceValidMoves(manager->game, pos, true, false);

	for (int i = 0; i < arrayListSize(validMoves); i++)
	{
		moveOption* moveOption = arrayListGetAt(validMoves, i);
		printf("<%d,%c>", moveOption->move.to.x + 1, moveOption->move.to.y + 'A');

		if (moveOption->isThreatened)
			printf("*");

		if (moveOption->isCapturing)
			printf("^");

		printf("\n");
	}
	if (command.arg != NULL)
		free(command.arg);
	arrayListDestroy(validMoves);
}

void exitGame(ChessGameManager* manager, bool isMallocError)
{
	gameManagerDestroy(manager);

	if (!isMallocError)
		printf("Exiting...\n");

	exit(0);
}

char* getPlayerColorName(PLAYER_COLOR color)
{
	return (color == WHITE ? "white" : "black");
}

void printWinner(ChessGameManager* manager)
{
	switch (manager->game->status)
	{
	case STATUS_MATE:
		printf("Checkmate! %s player wins the game\n", getPlayerColorName(getOpositeColor(manager->game->currentPlayer)));
		break;
	case STATUS_DRAW:
		printf("The game ends in a draw\n");
		break;
	case STATUS_CHECK:
		printf("Check: %s king is threatened\n", getPlayerColorName(manager->game->currentPlayer));//make_move switched the current player
	}
}

bool makeUserTurn(ChessGameManager* manager)
{
	gameManagerPrintBoard(manager);
	printf("Enter your move (%s player):\n", getPlayerColorName(manager->game->currentPlayer));

	ParsedCommand curCommand = getCommand();

	switch (curCommand.cmd)
	{
	case COMMAND_MOVE:
		ExecutionCommandMove(manager, curCommand);
		return false;
	case COMMAND_GET_MOVES:
		ExecutionGetMoves(manager, curCommand);
		return false;
	case COMMAND_SAVE://TODO
		if (saveToFile(curCommand.arg, manager) == false)
			printf("File cannot be created or modified\n");
		else
			printf("Game saved to : %s\n", curCommand.arg);
		return false;
	case COMMAND_UNDO:
		ExecutionCommandUndo(manager);
		return false;
	case COMMAND_RESET:
		gameDestroy(manager->game);
		manager->game = gameCreate();
		circularArrayClear(manager->history);
		gameManagerSetDefault(manager);
		printf("Specify game settings or type 'start' to begin a game with the current settings:\n");
		return true;
	case COMMAND_QUIT:
		exitGame(manager, false);
	case COMMAND_INVALID_LINE:
		printf("ERROR: invalid command\n");
		return false;

	}
	free(curCommand.arg);

	return false;
}

void printComputerMove(ChessGameManager* manager, Move move)
{
	ChessPiece* piece = gameGetPieceAt(manager->game, move.from);
	char* pieceString = "";
	switch (piece->type)
	{
	case PAWN:
		pieceString = "pawn";
		break;
	case KNIGHT:
		pieceString = "knight";
		break;
	case BISHOP:
		pieceString = "bishop";
		break;
	case ROOK:
		pieceString = "rook";
		break;
	case QUEEN:
		pieceString = "queen";
		break;
	case KING:
		pieceString = "king";
	}
	printf("Computer: move %s at <%d,%c> to <%d,%c>\n", pieceString, move.from.x + 1, move.from.y + 'A', move.to.x + 1, move.to.y + 'A');
}

void makeComputerTurn(ChessGameManager* manager)
{
	Move move = minimaxSuggestMove(manager->game, manager->difficulty + 1);
	//if (move == )//?
	//	exitGame(game, true);

	printComputerMove(manager, move);
	gameManagerMakeMove(manager, move);
	printWinner(manager);
}

PIECE_TYPE charToPieceType(char c)
{
	switch (c)
	{
	case 'm':
	case 'M':
		return PAWN;
	case 'b':
	case 'B':
		return BISHOP;
	case 'r':
	case 'R':
		return ROOK;
	case 'n':
	case 'N':
		return KNIGHT;
	case 'q':
	case 'Q':
		return QUEEN;
	case 'k':
	case 'K':
		return KING;

	}
	return PAWN;
}

void loadBoard(ChessGameManager* manager, FILE* f)
{
	char* c = (char*)malloc(sizeof(char) * 25);

	for (int j = 7; j >= 0; j--)
	{
		fgets(c, 25, f);
		removeSpaces(c);
		for (int i = 2; i <= 9; i++)
		{
			if (c[i] == ' ')
				continue;

			Position pos = newPos(j, i - 2);
			if (c[i] == '_')
			{
				if (manager->game->gameBoard[pos.x][pos.y] != NULL)
				{
					free(manager->game->gameBoard[pos.x][pos.y]);
					manager->game->gameBoard[pos.x][pos.y] = NULL;
				}
			}
			else
			{
				ChessPiece* piece = (ChessPiece*)malloc(sizeof(ChessPiece));
				if (piece == NULL)
				{
					mallocError = true;
					break;
				}
				piece->position = pos;
				piece->type = charToPieceType(c[i]);
				if (manager->game->gameBoard[pos.x][pos.y] != NULL)
					free(manager->game->gameBoard[pos.x][pos.y]);
				gameSetPieceAt(manager->game, pos, piece);
				if (c[i] > 'a')
					piece->color = WHITE;
				else
					piece->color = BLACK;
			}
		}
	}
	if (c != NULL)
		free(c);
}

void loadFile(char* filePath, ChessGameManager* manager)
{
	FILE* f;
	char* c = (char*)malloc(sizeof(char) * 25);
	gameManagerSetDefault(manager);

	if ((f = fopen(filePath, "r")) == NULL)
		return;

	if (strcmp(fgets(c, 25, f), "white\n") == 0)//sets current turn
		manager->game->currentPlayer = WHITE;
	else
		manager->game->currentPlayer = BLACK;

	fgets(c, 25, f);//useless line

	if (strcmp(fgets(c, 25, f), "GAME_MODE: 2-player\n") == 0)
	{
		manager->mode = TWO_PLAYERS;
		loadBoard(manager, f);
	}
	else
	{
		manager->difficulty = difficultyStringToArg(fgets(c, 25, f));
		if (strcmp(fgets(c, 25, f), "USER_COLOR: black\n") == 0)
			manager->computerColor = WHITE;

		loadBoard(manager, f);
	}
	free(c);
	fclose(f);
}

void saveBoard(ChessGameManager* manager, FILE* f)
{
	if (manager->game == NULL)
		return;

	for (int j = 7; j >= 0; j--)
	{
		fprintf(f, "%d| ", j + 1);
		for (int i = 0; i < 8; i++)
		{
			fprintf(f, "%c ", printPiece(gameGetPieceAt(manager->game, newPos(j, i))));
		}
		fprintf(f, "|\n");
	}
	fprintf(f, "  -----------------\n");
	fprintf(f, "   A B C D E F G H\n");
}

bool saveToFile(char* filePath, ChessGameManager* manager)
{
	FILE *f;
	char* currentPlayer = getPlayerColorName(manager->game->currentPlayer);
	char* difficulty= (char*)malloc(sizeof(char));
	char* userColor;

	if ((f = fopen(filePath, "w")) == NULL)
		return false;

	fprintf(f, "%s\n", currentPlayer);
	fprintf(f, "SETTINGS:\n");

	if (manager->mode == ONE_PLAYER)
	{
		fprintf(f, "GAME_MODE: 1-player\n");
		difficulty[0] = (manager->difficulty + 1) + '0';
		difficulty = difficultyArgPrint(difficulty);
		fprintf(f, "DIFFICULTY: %s\n", difficulty);
		userColor = getPlayerColorName(manager->computerColor);
		fprintf(f, "USER_COLOR: %s\n", userColor);
		saveBoard(manager, f);
	}
	else
	{
		fprintf(f, "GAME_MODE: 2-player\n");
		saveBoard(manager, f);

	}
	fclose(f);
	return true;
}