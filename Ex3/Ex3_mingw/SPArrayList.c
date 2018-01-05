#include <stdio.h>
#include "SPArrayList.h"
#include <stdlib.h>
#include "SPMainAux.h"

SPArrayList* spArrayListCreate(int maxSize)
{
	SPArrayList *newList = (SPArrayList*)malloc(sizeof(SPArrayList));
	if (newList == NULL)
	{
		printf("Error: spArrayListCreate has failed");
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
			printf("Error: spArrayListCreate has failed");
			return NULL;
		}

		return newList;
	}
}

SPArrayList* spArrayListCopy(SPArrayList* src) {
	SPArrayList* copy;
	copy = (SPArrayList*)malloc(sizeof(SPArrayList));

	if (copy == NULL)
	{
		printf("Error: spArrayListCopy has failed");
		return NULL;
	}

	copy->maxSize = src->maxSize;
	copy->actualSize = src->actualSize;
	copy->elements = (int*)malloc(sizeof(int) * (src->maxSize));

	if (copy->elements == NULL || src == NULL)
	{
		free(copy);
		printf("Error: spArrayListCopy has failed");
		return NULL;
	}
	else
	{
		for (int i = 0; i < src->maxSize; i++)
			copy->elements[i] = src->elements[i];
	}

	return copy;
}

void spArrayListDestroy(SPArrayList* src)
{
	if (src != NULL)
	{
		if (src->elements != NULL)
			free(src->elements);

		free(src);
	}
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src)
{
	if (src == NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	for (int i = 0; i < src->actualSize; i++)
		src->elements[i] = 0;

	src->actualSize = 0;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index)
{
	if (src == NULL || src->actualSize < index)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	if ((src->actualSize) + 1 > src->maxSize)
		return SP_ARRAY_LIST_FULL;

	for (int i = 0; i < ((src->actualSize) - index); i++)
	{
		src->elements[(src->actualSize) - i] = src->elements[(src->actualSize) - i - 1];
	}

	src->elements[index] = elem;
	src->actualSize++;

	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem)
{
	return spArrayListAddAt(src, elem, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem)
{
	return spArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index)
{
	if (src == NULL || src->actualSize <= index)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	if (src->actualSize == 0)
		return SP_ARRAY_LIST_EMPTY;

	for (int i = 0; i < ((src->actualSize) - index - 1); i++)
	{
		src->elements[index + i] = src->elements[index + i + 1];
	}

	src->elements[src->actualSize - 1] = 0;
	src->actualSize--;

	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src)
{
	return spArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src)
{
	return spArrayListRemoveAt(src, (src->actualSize) - 1);
}

int spArrayListGetAt(SPArrayList* src, int index)
{
	if (src == NULL)
		return 0;

	if (index < 0 || index > src->actualSize)
		return 0;

	return src->elements[index];
}

int spArrayListGetFirst(SPArrayList* src)
{
	return src->elements[0];
}

int spArrayListGetLast(SPArrayList* src)
{
	return src->elements[src->actualSize - 1];
}

int spArrayListMaxCapacity(SPArrayList* src)
{
	return src->maxSize;
}

int spArrayListSize(SPArrayList* src)
{
	return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src)
{
	return src->actualSize == src->maxSize;
}

bool spArrayListIsEmpty(SPArrayList* src)
{
	return src->actualSize == 0;
}