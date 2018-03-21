#include <stdio.h>
#include "ChessGame.h"


int main()
{
	ChessGame* g = gameCreate(2);
	gamePrintBoard(g);
	getchar();
	return 0;
}