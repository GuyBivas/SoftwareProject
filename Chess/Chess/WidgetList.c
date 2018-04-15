#include "WidgetList.h"

WidgetList* widgetListCreate(int maxSize)
{
	WidgetList* newList = (WidgetList*)malloc(sizeof(WidgetList));
	if (newList == NULL)
	{
		mallocError = true;
		printf("Error: widgetListCreate has failed");
		return NULL;
	}

	if (maxSize <= 0)
	{
		return NULL;
	}
	else
	{
		newList->maxSize = maxSize;
		newList->elements = (Widget**)malloc(sizeof(Widget*) * maxSize);
		newList->actualSize = 0;

		if (newList->elements == NULL)
		{
			mallocError = true;
			free(newList);
			printf("Error: widgetListCreate has failed");
			return NULL;
		}

		return newList;
	}
}

void widgetListDestroy(WidgetList* src)
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

void widgetListClear(WidgetList * src)
{
	if (src != NULL)
	{
		if (src->elements != NULL)
		{
			for (int i = 0; i < src->actualSize; i++)
			{
				widgetDestroy(src->elements[i]);
			}
		}

		src->actualSize = 0;
	}
}

ARRAY_LIST_MESSAGE widgetListAddAt(WidgetList* src, Widget* elem, int index)
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

ARRAY_LIST_MESSAGE widgetListAddFirst(WidgetList* src, Widget* elem)
{
	return widgetListAddAt(src, elem, 0);
}

ARRAY_LIST_MESSAGE widgetListAddLast(WidgetList* src, Widget* elem)
{
	return widgetListAddAt(src, elem, src->actualSize);
}

ARRAY_LIST_MESSAGE widgetListRemoveAt(WidgetList* src, int index)
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

ARRAY_LIST_MESSAGE widgetListRemoveFirst(WidgetList* src)
{
	return widgetListRemoveAt(src, 0);
}

ARRAY_LIST_MESSAGE widgetListRemoveLast(WidgetList* src)
{
	return widgetListRemoveAt(src, (src->actualSize) - 1);
}

Widget* widgetListGetAt(WidgetList* src, int index)
{
	if (src == NULL)
		return 0;

	if (index < 0 || index > src->actualSize)
		return 0;

	return src->elements[index];
}

Widget* widgetListGetFirst(WidgetList* src)
{
	return src->elements[0];
}

Widget* widgetListGetLast(WidgetList* src)
{
	return src->elements[src->actualSize - 1];
}

int widgetListMaxCapacity(WidgetList* src)
{
	return src->maxSize;
}

int widgetListSize(WidgetList* src)
{
	return src->actualSize;
}

bool widgetListIsFull(WidgetList* src)
{
	return src->actualSize == src->maxSize;
}

bool widgetListIsEmpty(WidgetList* src)
{
	return src->actualSize == 0;
}

ARRAY_LIST_MESSAGE widgetListAddList(WidgetList* src, WidgetList* addition)
{
	if (src->maxSize < (src->actualSize + addition->actualSize))
		return ARRAY_LIST_FULL;

	for (int i = 0; i < addition->actualSize; i++)
	{
		widgetListAddLast(src, widgetListGetAt(addition, i));
	}

	return ARRAY_LIST_SUCCESS;
}