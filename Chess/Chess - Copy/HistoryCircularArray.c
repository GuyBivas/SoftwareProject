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
			free(src->elements);

		free(src);
	}
}

void circularArrayAdd(HistoryCircularArray * src, ChessGame * elem)
{
}

ARRAY_LIST_MESSAGE circularArrayRemove(HistoryCircularArray * src)
{
	return ARRAY_LIST_SUCCESS;
}

int circularArrayGetCurrent(HistoryCircularArray * src)
{
	return 0;
}
