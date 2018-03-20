#include <stdio.h>
#include <stdlib.h>
#include "HistoryCircularArray.h"

HistoryCircularArray* circularArrayCreate(int maxSize)
{
	HistoryCircularArray* newList = (HistoryCircularArray*)malloc(sizeof(HistoryCircularArray));
	if (newList == NULL)
	{
		printf("Error: circularArrayCreate has failed");
		return NULL;
	}

	if (maxSize <= 0)
	{
		return NULL;
	}
	else
	{
		newList->maxSize = maxSize;
		newList->elements = (ChessGame**)malloc(sizeof(ChessGame*) * maxSize);
		newList->actualSize = 0;
		newList->index = 0;

		if (newList->elements == NULL)
		{
			free(newList);
			printf("Error: circularArrayCreate has failed");
			return NULL;
		}

		return newList;
	}
}

void circularArrayDestroy(HistoryCircularArray * src)
{
	if (src != NULL)
	{
		if (src->elements != NULL)
		{
			for (int i = 0; i < src->actualSize; i++)
			{
				free(src->elements[i]);
			}

			free(src->elements);
		}

		free(src);
	}
}

ChessGame* circularArrayGetCurrent(HistoryCircularArray* src)
{
	if (src->actualSize == 0)
		return NULL;

	return src->elements[src->index - 1];
}

void circularArrayAdd(HistoryCircularArray* src, ChessGame* elem)
{
	src->elements[src->index] = elem;

	src->index++;
	if (src->index >= src->maxSize)
		src->index = 0;

	src->actualSize++;
	if (src->actualSize > src->maxSize)
		src->actualSize = src->maxSize;
}

ARRAY_LIST_MESSAGE circularArrayRemove(HistoryCircularArray* src)
{
	if (src->actualSize == 0)
		return ARRAY_LIST_EMPTY;

	src->index--;
	if (src->index < 0)
		src->index = src->maxSize - 1;

	src->actualSize--;
	if (src->actualSize < 0)
		src->actualSize = 0;

	free(src->elements[src->index]); // decreased index 
}
