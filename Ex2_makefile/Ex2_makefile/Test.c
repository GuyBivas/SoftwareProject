#include <stdio.h>
#include "SPBufferset.h"
#include "main_aux.h"

#define N 1000

int main2() {
	int n = 0;
	int heapSizes[N];

	SP_BUFF_SET();

	printf("Enter the number of heaps:\n");
	scanf("%d", &n);

	if (n < 1 || n > 32)
	{
		printf("Error: the number of heaps must be between 1 and 32.\n");
		return 0;
	}

	printf("Enter the heap sizes:\n");
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &heapSizes[i]);
		if (heapSizes[i] <= 0)
		{
			printf("Error: the size of heap %d should be positive.\n", i);
			return 0;
		}
	}

	RunGame(heapSizes, n);
}