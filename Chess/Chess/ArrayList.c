#include <stdio.h>
#include "ArrayList.h"
#include <stdlib.h>

ArrayList* ArrayListCreate(int maxSize)
{
	ArrayList *newList = (ArrayList*)malloc(sizeof(ArrayList));
	if (newList == NULL)
	{
		printf("Error: ArrayListCreate has failed");
		return NULL;
	}

	if (maxSize <= 0)
	{
		return NULL;
	}
	else
	{
		newList->maxSize = maxSize;
		newList->elements = (int*)malloc(sizeof(int) * maxSize);
		newList->actualSize = 0;

		if (newList->elements == NULL)
		{
			free(newList);
			printf("Error: ArrayListCreate has failed");
			return NULL;
		}

		return newList;
	}
}

ArrayList* ArrayListCopy(ArrayList* src) {
	ArrayList* copy;
	copy = (ArrayList*)malloc(sizeof(ArrayList));

	if (copy == NULL)
	{
		printf("Error: ArrayListCopy has failed");
		return NULL;
	}

	copy->maxSize = src->maxSize;
	copy->actualSize = src->actualSize;
	copy->elements = (int*)malloc(sizeof(int) * (src->maxSize));

	if (copy->elements == NULL || src == NULL)
	{
		free(copy);
		printf("Error: ArrayListCopy has failed");
		return NULL;
	}
	else
	{
		for (int i = 0; i < src->maxSize; i++)
			copy->elements[i] = src->elements[i];
	}

	return copy;
}

void ArrayListDestroy(ArrayList* src)
{
	if (src != NULL)
	{
		if (src->elements != NULL)
			free(src->elements);

		free(src);
	}
}

ARRAY_LIST_MESSAGE ArrayListClear(ArrayList* src)
{
	if (src == NULL)
		return ARRAY_LIST_INVALID_ARGUMENT;

	for (int i = 0; i < src->actualSize; i++)
		src->elements[i] = 0;

	src->actualSize = 0;
	return ARRAY_LIST_SUCCESS;
}

ARRAY_LIST_MESSAGE ArrayListAddAt(ArrayList* src, int elem, int index)
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

ARRAY_LIST_MESSAGE ArrayListAddFirst(ArrayList* src, int elem)
{
	return ArrayListAddAt(src, elem, 0);
}

ARRAY_LIST_MESSAGE ArrayListAddLast(ArrayList* src, int elem)
{
	return ArrayListAddAt(src, elem, src->actualSize);
}

ARRAY_LIST_MESSAGE ArrayListRemoveAt(ArrayList* src, int index)
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

ARRAY_LIST_MESSAGE ArrayListRemoveFirst(ArrayList* src)
{
	return ArrayListRemoveAt(src, 0);
}

ARRAY_LIST_MESSAGE ArrayListRemoveLast(ArrayList* src)
{
	return ArrayListRemoveAt(src, (src->actualSize) - 1);
}

int ArrayListGetAt(ArrayList* src, int index)
{
	if (src == NULL)
		return 0;

	if (index < 0 || index > src->actualSize)
		return 0;

	return src->elements[index];
}

int ArrayListGetFirst(ArrayList* src)
{
	return src->elements[0];
}

int ArrayListGetLast(ArrayList* src)
{
	return src->elements[src->actualSize - 1];
}

int ArrayListMaxCapacity(ArrayList* src)
{
	return src->maxSize;
}

int ArrayListSize(ArrayList* src)
{
	return src->actualSize;
}

bool ArrayListIsFull(ArrayList* src)
{
	return src->actualSize == src->maxSize;
}

bool ArrayListIsEmpty(ArrayList* src)
{
	return src->actualSize == 0;
}