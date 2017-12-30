#include <stdio.h>
#include <stdbool.h>
#include "sp_nim.h"
#include "main_aux.h"

void RunGame(int *heapSizes, int n)
{
	int turn = 1;
	while (true)
	{
		PrintState(heapSizes, n, turn++);
		ComputerMove(heapSizes, n);
		if (CheckWin(heapSizes, n))
		{
			printf("Computer wins!\n");
			return;
		}

		PrintState(heapSizes, n, turn++);
		PlayerMove(heapSizes, n);
		if (CheckWin(heapSizes, n))
		{
			printf("You win!\n");
			return;
		}
	}
}

void PrintState(int *heapSizes, int n, int turn)
{
	printf("In turn %d heap sizes are: ", turn);
	for (int i = 0; i < n; i++)
	{
		printf(" h%d=%d", i + 1, heapSizes[i]);
	}

	printf(".\n");
}