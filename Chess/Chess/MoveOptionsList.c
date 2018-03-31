#include <stdio.h>
#include <stdlib.h>
#include "MoveOptionsList.h"

MoveOptionsList* arrayListCreate(int maxSize)
{
	MoveOptionsList* newList = (MoveOptionsList*)malloc(sizeof(MoveOptionsList));
	if (newList == NULL)
	{
		printf("Error: arrayListCreate has failed");
		return NULL;
	}

	if (maxSize <= 0)
	{
		return NULL;
	}
	else
	{
		newList->maxSize = maxSize;
		newList->elements = (moveOption**)malloc(sizeof(moveOption*) * maxSize);
		newList->actualSize = 0;

		if (newList->elements == NULL)
		{
			free(newList);
			printf("Error: arrayListCreate has failed");
			return NULL;
		}

		return newList;
	}
}

void arrayListDestroy(MoveOptionsList* src)
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

ARRAY_LIST_MESSAGE arrayListAddAt(MoveOptionsList* src, moveOption* elem, int index)
{
	if (src == NULL || src->actualSize < index)
		return ARRAY_LIST_INVALID_ARGUMENT;

	if ((src->actualSize) + 1 > src->maxSize)
		return ARRAY_LIST_FULL;

	for (int i = 0; i < ((src->actualSize) - index); i++)
	{
		src->elements[(src->actualSize) - i] = src->elements[(src->actualSize) - i - 1];
	}

	src->elements[index] = elem;
	src->actualSize++;

	return ARRAY_LIST_SUCCESS;
}

ARRAY_LIST_MESSAGE arrayListAddFirst(MoveOptionsList* src, moveOption* elem)
{
	return arrayListAddAt(src, elem, 0);
}

ARRAY_LIST_MESSAGE arrayListAddLast(MoveOptionsList* src, moveOption* elem)
{
	return arrayListAddAt(src, elem, src->actualSize);
}

ARRAY_LIST_MESSAGE arrayListRemoveAt(MoveOptionsList* src, int index)
{
	if (src == NULL || src->actualSize <= index)
		return ARRAY_LIST_INVALID_ARGUMENT;

	if (src->actualSize == 0)
		return ARRAY_LIST_EMPTY;

	for (int i = 0; i < ((src->actualSize) - index - 1); i++)
	{
		src->elements[index + i] = src->elements[index + i + 1];
	}

	src->elements[src->actualSize - 1] = 0;
	src->actualSize--;

	return ARRAY_LIST_SUCCESS;
}

ARRAY_LIST_MESSAGE arrayListRemoveFirst(MoveOptionsList* src)
{
	return arrayListRemoveAt(src, 0);
}

ARRAY_LIST_MESSAGE arrayListRemoveLast(MoveOptionsList* src)
{
	return arrayListRemoveAt(src, (src->actualSize) - 1);
}

moveOption* arrayListGetAt(MoveOptionsList* src, int index)
{
	if (src == NULL)
		return 0;

	if (index < 0 || index > src->actualSize)
		return 0;

	return src->elements[index];
}

moveOption* arrayListGetFirst(MoveOptionsList* src)
{
	return src->elements[0];
}

moveOption* arrayListGetLast(MoveOptionsList* src)
{
	return src->elements[src->actualSize - 1];
}

int arrayListMaxCapacity(MoveOptionsList* src)
{
	return src->maxSize;
}

int arrayListSize(MoveOptionsList* src)
{
	return src->actualSize;
}

bool arrayListIsFull(MoveOptionsList* src)
{
	return src->actualSize == src->maxSize;
}

bool arrayListIsEmpty(MoveOptionsList* src)
{
	return src->actualSize == 0;
}

//bool arrayListContains(MoveOptionsList* src, Position pos)
//{
//	for (int i = 0; i < src->actualSize; i++)
//	{
//		if ((src->elements[i])->pos.x == pos.x && (src->elements[i])->pos.y == pos.y)
//			return true;
//	}
//
//	return false;
//}