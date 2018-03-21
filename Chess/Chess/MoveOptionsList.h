#ifndef MOVEOPTIONSLIST_H
#define MOVEOPTIONSLIST_H

#include <stdbool.h>
#include "Enums.h"

/**
 * ArrayList summary:
 *
 * A container that represents a fixed size linked list. The capcity of the list
 * is ecified at the creation. The container supports typical list
 * functionalities with the addition of random access as in arrays.
 * Upon insertion, if the maximum capacity is reached then an error message is
 * returned and the list is not affected. A summary of the supported functions
 * is given below:
 *
 * ArrayListCreate       - Creates an empty array list with a ecified
 *                           max capacity.
 * ArrayListCopy         - Creates an exact copy of a ecified array list.
 * ArrayListDestroy      - Frees all memory resources associated with an array
 *                           list.
 * ArrayListClear        - Clears all elements from a ecified array list.
 * ArrayListAddAt        - Inserts an element at a ecified index, elements
 *                           will be shifted to make place.
 * ArrayListAddFirst     - Inserts an element at the beginning of the array
 *                           list, elements will be shifted to make place.
 * ArrayListAddLast      - Inserts an element at the end of the array list.
 * ArrayListRemoveAt     - Removes an element at the ecified index, elements
 *                           elements will be shifted as a result.
 * ArrayListRemoveFirst  - Removes an element from the beginning of the array
 *                           list, elements will be shifted as a result.
 * ArrayListRemoveLast   - Removes an element from the end of the array list
 * ArrayListGetAt        - Accesses the element at the ecified index.
 * ArrayListGetFirst     - Accesses the first element of the array list.
 * ArrayListGetLast      - Accesses the last element of the array list.
 * ArrayListMaxCapcity   - Returns the maximum capcity of the array list.
 * ArrayListSize         - Returns the number of elements in that array list.
 * ArrayListIsFull       - Returns if the array list reached its max capacity.
 * ArrayListIsEmpty      - Returns true if the array list contains no elements.
 */

typedef struct moveOption moveOption;

typedef struct move_options_list {
	moveOption** elements;
	int actualSize;
	int maxSize;
} MoveOptionsList;

/**
 *  Creates an empty array list with the ecified maximum capacity.
 *  @param maxSize - the maximum capacity of the target array list.
 *  @return
 *  NULL, if an allocation error occurred or maxSize  <= 0.
 *  An instant of an array list otherwise.
 */
MoveOptionsList* arrayListCreate(int maxSize);

/**
 * Frees all memory resources associated with the source array list. If the
 * source array is NULL, then the function does nothing.
 * @param src - the source array list
 */
void arrayListDestroy(MoveOptionsList* src);

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
ARRAY_LIST_MESSAGE arrayListAddAt(MoveOptionsList* src, moveOption* elem, int index);

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
 ARRAY_LIST_MESSAGE arrayListAddFirst(MoveOptionsList* src, moveOption* elem);

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
ARRAY_LIST_MESSAGE arrayListAddLast(MoveOptionsList* src, moveOption* elem);

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
ARRAY_LIST_MESSAGE arrayListRemoveAt(MoveOptionsList* src, int index);

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
ARRAY_LIST_MESSAGE arrayListRemoveFirst(MoveOptionsList* src);

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
ARRAY_LIST_MESSAGE arrayListRemoveLast(MoveOptionsList* src);

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
moveOption* arrayListGetAt(MoveOptionsList* src, int index);

/**
 * Returns the element at the beginning of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the beginning of the list is returned.
 */
moveOption* arrayListGetFirst(MoveOptionsList* src);

/**
 * Returns the element at the end of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the end of the list is returned.
 */
moveOption* arrayListGetLast(MoveOptionsList* src);

/**
 * Returns the maximum capacity of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the maximum capacity of the list is returned.
 */
int arrayListMaxCapacity(MoveOptionsList* src);

/**
 * Returns the number of elements in the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the number of the elements in the list is returned.
 */
int arrayListSize(MoveOptionsList* src);

/**
 * Returns true if the list is full, that is the number of elements in the list
 * equals its maximum capacity.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is less
 * than its maximum capacity.
 * Otherwise, true is returned.
 */
bool arrayListIsFull(MoveOptionsList* src);

/**
 * Returns true if the list is empty, that is the number of elements in the list
 * equals to zero.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is not zero.
 * Otherwise, true is returned.
 */
bool arrayListIsEmpty(MoveOptionsList* src);

//bool arrayListContains(MoveOptionsList* src, Position pos);

#endif
