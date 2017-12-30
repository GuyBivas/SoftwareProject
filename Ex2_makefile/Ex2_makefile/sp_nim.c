#include <stdio.h>
#include <stdbool.h>
#include "sp_nim.h"

int SnimCalculate(int* heapSizes, int n)
{
	int sNim = heapSizes[0];
	for (int i = 1; i < n; i++)
		sNim = (sNim ^ heapSizes[i]);

	return sNim;

	/*int sNim = 0;
	for (int i = 0; i < n; i++)
		sNim ^= heapSizes[i];

	return sNim;*/
}

void ComputerMove(int *heapSizes, int n)
{
	int snim = SnimCalculate(heapSizes, n);
	int count;

	for (int i = 0; i < n; i++)
	{
		int size = heapSizes[i];

		if (snim == 0)
		{
			if (size > 0)
			{
				count = 1;
				printf("Computer takes %d objects from heap %d.\n", count, i + 1);
				heapSizes[i]--;

				return;
			}
		}
		else
		{
			if ((size ^ snim) < size)
			{
				count = heapSizes[i] - (size ^ snim);
				printf("Computer takes %d objects from heap %d.\n", count, i + 1);
				heapSizes[i] = (size ^ snim);

				return;
			}
		}
	}
}

void PlayerMove(int* heapSizes, int n)
{
	int i, numOfTokens = 0;
	bool valid = false;
	while (!valid)
	{
		printf("Your turn: please enter the heap index and the number of removed objects.\n");
		scanf("%d %d", &i, &numOfTokens);
		if ((i <= n && i > 0) && (numOfTokens > 0 && numOfTokens <= heapSizes[i-1]))
		{
			heapSizes[i-1] -= numOfTokens;
			printf("You take %d objects from heap %d.\n", numOfTokens, i);
			
			valid = true;
		}
		else
			printf("Error: Invalid input.\nPlease enter again the heap index and the number of removed objects.\n");
	}
}

bool CheckWin(int *heapSizes, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (heapSizes[i] != 0)
			return false;
	}

	return true;
}