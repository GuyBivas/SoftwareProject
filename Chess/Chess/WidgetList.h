#ifndef WIDGETLIST_H
#define WIDGETLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Position.h"
#include "Enums.h"
#include "Widget.h"

/**
* widgetList summary:
*
* A container that represents a fixed size linked list. The capcity of the list
* is ecified at the creation. The container supports typical list
* functionalities with the addition of random access as in arrays.
* Upon insertion, if the maximum capacity is reached then an error message is
* returned and the list is not affected. A summary of the supported functions
* is given below:
*
* widgetListCreate       - Creates an empty array list with a ecified
*                           max capacity.
* widgetListCopy         - Creates an exact copy of a ecified array list.
* widgetListDestroy      - Frees all memory resources associated with an array
*                           list.
* widgetListClear        - Clears all elements from a ecified array list.
* widgetListAddAt        - Inserts an element at a ecified index, elements
*                           will be shifted to make place.
* widgetListAddFirst     - Inserts an element at the beginning of the array
*                           list, elements will be shifted to make place.
* widgetListAddLast      - Inserts an element at the end of the array list.
* widgetListRemoveAt     - Removes an element at the ecified index, elements
*                           elements will be shifted as a result.
* widgetListRemoveFirst  - Removes an element from the beginning of the array
*                           list, elements will be shifted as a result.
* widgetListRemoveLast   - Removes an element from the end of the array list
* widgetListGetAt        - Accesses the element at the ecified index.
* widgetListGetFirst     - Accesses the first element of the array list.
* widgetListGetLast      - Accesses the last element of the array list.
* widgetListMaxCapcity   - Returns the maximum capcity of the array list.
* widgetListSize         - Returns the number of elements in that array list.
* widgetListIsFull       - Returns if the array list reached its max capacity.
* widgetListIsEmpty      - Returns true if the array list contains no elements.
*/

typedef struct widget_list {
	Widget** elements;
	int actualSize;
	int maxSize;
} WidgetList;

/**
*  Creates an empty array list with the ecified maximum capacity.
*  @param maxSize - the maximum capacity of the target array list.
*  @return
*  NULL, if an allocation error occurred or maxSize  <= 0.
*  An instant of an array list otherwise.
*/
WidgetList* widgetListCreate(int maxSize);

/**
* Frees all memory resources associated with the source array list. If the
* source array is NULL, then the function does nothing.
* @param src - the source array list
*/
void widgetListDestroy(WidgetList* src);

void widgetListClear(WidgetList * src);

/**
* Inserts element at a ecified index. The elements residing at and after the
* ecified index will be shifted to make place for the new element. If the
* array list reached its maximum capacity and error message is returned and
* the source list is not affected
* @param src   - the source array list
* @param elem  - the new element to be inserted
* @param index - the index where the new element will be placed. The index is
*                0-based.
* @return
* ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
* ARRAY_LIST_FULL - if the source array list reached its maximum capacity
* ARRAY_LIST_SUCCESS - otherwise
*/
ARRAY_LIST_MESSAGE widgetListAddAt(WidgetList* src, Widget* elem, int index);

/**
* Inserts element at a the beginning of the source element. The elements
* will be shifted to make place for the new element. If the
* array list reached its maximum capacity and error message is returned and
* the source list is not affected
* @param src   - the source array list
* @param elem  - the new element to be inserted
* @return
* ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
* ARRAY_LIST_FULL - if the source array list reached its maximum capacity
* ARRAY_LIST_SUCCESS - otherwise
*/
ARRAY_LIST_MESSAGE widgetListAddFirst(WidgetList* src, Widget* elem);

/**
* Inserts element at a the end of the source element. If the array list
* reached its maximum capacity and error message is returned and the source
* list is not affected.
* @param src   - the source array list
* @param elem  - the new element to be inserted
* @return
* ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
* ARRAY_LIST_FULL - if the source array list reached its maximum capacity
* ARRAY_LIST_SUCCESS - otherwise
*/
ARRAY_LIST_MESSAGE widgetListAddLast(WidgetList* src, Widget* elem);

/**
* Removes an element from a ecified index. The elements residing after the
* ecified index will be shifted to make to keep the list continuous. If the
* array list is empty then an error message is returned and the source list
* is not affected
* @param src   - The source array list
* @param index - The index from where the element will be removed.
*                The index is 0-based.
* @return
* ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
* ARRAY_LIST_EMPTY - if the source array list is empty
* ARRAY_LIST_SUCCESS - otherwise
*/
ARRAY_LIST_MESSAGE widgetListRemoveAt(WidgetList* src, int index);

/**
* Removes an element from a the beginning of the list.
* The elements will be shifted to make to keep the list continuous. If the
* array list is empty then an error message is returned and the source list
* is not affected
* @param src   - The source array list
* @param elem  - The new element to be inserted
* @return
* ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
* ARRAY_LIST_EMPTY - if the source array list is empty
* ARRAY_LIST_SUCCESS - otherwise
*/
ARRAY_LIST_MESSAGE widgetListRemoveFirst(WidgetList* src);

/**
* Removes an element from a the end of the list.
* The elements will be shifted to make to keep the list continuous. If the
* array list is empty then an error message is returned and the source list
* is not affected
* @param src   - The source array list
* @param elem  - The new element to be inserted
* @return
* ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
* ARRAY_LIST_EMPTY - if the source array list is empty
* ARRAY_LIST_SUCCESS - otherwise.
*/
ARRAY_LIST_MESSAGE widgetListRemoveLast(WidgetList* src);

/**
* Returns the element at the ecified index. The function is called
* with the assertion that all arguments are valid. If any of the arguments is
* invalid then an undefined value is returned.
* @param src - the source array list
* @param index - the ecified index, the index is 0-based.
* @return
* Undefined value if either src == NULL or index out of bound.
* Otherwise, the element at the ecified index is returned.
*/
Widget* widgetListGetAt(WidgetList* src, int index);

/**
* Returns the element at the beginning of the list. The function is called
* with the assertion that all arguments are valid. If any of the arguments is
* invalid then an undefined value is returned.
* @param src - the source array list
* @return
* Undefined value if either src == NULL or the list is empty
* Otherwise, the element at the beginning of the list is returned.
*/
Widget* widgetListGetFirst(WidgetList* src);

/**
* Returns the element at the end of the list. The function is called
* with the assertion that all arguments are valid. If any of the arguments is
* invalid then an undefined value is returned.
* @param src - the source array list
* @return
* Undefined value if either src == NULL or the list is empty
* Otherwise, the element at the end of the list is returned.
*/
Widget* widgetListGetLast(WidgetList* src);

/**
* Returns the maximum capacity of the list. The function is called
* with the assertion that all arguments are valid. If any of the arguments is
* invalid then an undefined value is returned.
* @param src - the source array list
* @return
* Undefined value if either src == NULL
* Otherwise, the maximum capacity of the list is returned.
*/
int widgetListMaxCapacity(WidgetList* src);

/**
* Returns the number of elements in the list. The function is called
* with the assertion that all arguments are valid. If any of the arguments is
* invalid then an undefined value is returned.
* @param src - the source array list
* @return
* Undefined value if either src == NULL
* Otherwise, the number of the elements in the list is returned.
*/
int widgetListSize(WidgetList* src);

/**
* Returns true if the list is full, that is the number of elements in the list
* equals its maximum capacity.
* @param src - the source array list
* @return
* false if either src == NULL or the number of elements in the list is less
* than its maximum capacity.
* Otherwise, true is returned.
*/
bool widgetListIsFull(WidgetList* src);

/**
* Returns true if the list is empty, that is the number of elements in the list
* equals to zero.
* @param src - the source array list
* @return
* false if either src == NULL or the number of elements in the list is not zero.
* Otherwise, true is returned.
*/
bool widgetListIsEmpty(WidgetList* src);

#endif
