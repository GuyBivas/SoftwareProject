#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "MainAux.h"
#include "Minimax.h"

ParsedCommand getCommand()
{
	ParsedCommand cmd;
	char command[MAX_LINE_LENGTH];
	fgets(command, MAX_LINE_LENGTH, stdin);
	cmd = ParserPraseLine(command);

	return cmd;
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
		//	case COMMAND_LOAD://continue tommorow from here
		//		return; // "load";
	case COMMAND_DEFAULT:
		manager->mode = ONE_PLAYER;
		manager->difficulty = EASY;
		manager->game->currentPlayer = WHITE;
		return true;
	case COMMAND_QUIT:
		exitGame(manager, false);
		return true;
	case COMMAND_START:
		printf("Starting game...\n");
		return false; // "start";
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
		printf("Wrong difficulty level. The value should be between 1 to 5\n");
	}
}

void ExecutionCommandUserColor(ChessGameManager* manager, ParsedCommand command)
{
	if (manager->mode == ONE_PLAYER)
	{
		if (command.validArg == true)
			printf("User color is set to %s\n", enumArgToString(command));
		else
			printf("Wrong user color. The value should be 0 or 1\n");
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

	if (manager->game->status == STATUS_NORMAL || manager->game->status == STATUS_CHECK)
		currentPlayer = 1 - (manager->game->currentPlayer); // todo

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position pos = newPos(i, j);
			ChessPiece* currentPiece = gameGetPieceAt(manager->game, pos);
			ChessPiece* previousPiece = gameGetPieceAt(beforeLastMove, pos);

			if (currentPiece->color != currentPlayer)
				continue;

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

	if (manager->game->status == STATUS_MATE || manager->game->status == STATUS_DRAW)
		currentPlayer = manager->game->currentPlayer;
	else
		currentPlayer = 1 - (manager->game->currentPlayer);

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
	else if (circularArrayListSize(manager->history) == 1 || (manager->game->status == STATUS_MATE && manager->game->currentPlayer == WHITE && manager->mode == ONE_PLAYER))//TODO change WHITE
	{
		k = 1;
	}

	for (int i = 0; i < k; i++)
	{
		lastMove = getLastMove(manager);
		printLastMove(manager, &lastMove);
		gameManagerUndoPrevMove(manager);
		gameManagerPrintBoard(manager);
	}
}

void ExecutionCommandMove(ChessGameManager* manager, ParsedCommand command)
{
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
	}

	if (manager->game->status == STATUS_MATE || manager->game->status == STATUS_DRAW)
	{
		printWinner(manager);
		exitGame(manager, true);
	}
}

void ExecutionGetMoves(ChessGameManager* manager, ParsedCommand command)//TODO
{
	int x = command.arg[1] - '0';
	int y = command.arg[3] - '0';
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

	MoveOptionsList* validMoves = gameGetPieceValidMoves(manager->game, pos, true);

	for (int i = 0; i < arrayListSize(validMoves); i++)
	{
		moveOption* moveOption = arrayListGetAt(validMoves, i);
		printf("<%d,%d>", moveOption->move.to.x, moveOption->move.to.y);

		if (moveOption->isThreatened)
			printf("*");

		if (moveOption->isCapturing)
			printf("^");

		printf("\n");
	}

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
		printf("Checkmate! %s player wins the game\n", getPlayerColorName(manager->game->currentPlayer));
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
		return false;
	case COMMAND_UNDO:
		ExecutionCommandUndo(manager);
		return false;
	case COMMAND_RESET://TODO
		printf("Specify game settings or type 'start' to begin a game with the current settings:\n");
		return true;
	case COMMAND_QUIT:
		exitGame(manager, false);
	case COMMAND_INVALID_LINE:
		printf("ERROR: invalid command\n");
		return false;

	}

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