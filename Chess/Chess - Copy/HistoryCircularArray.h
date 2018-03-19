#ifndef HISTORYCIRCULARARRAY_H_
#define HISTORYCIRCULARARRAY_H_

#include <stdbool.h>
#include "Enums.h"

typedef struct ChessGame ChessGame;

typedef struct history_circular_array {
	ChessGame** elements;
	int actualSize;
	int maxSize;
	int index;
} HistoryCircularArray;

HistoryCircularArray* circularArrayCreate(int maxSize);

void circularArrayDestroy(HistoryCircularArray* src);

void circularArrayAdd(HistoryCircularArray* src, ChessGame* elem);

ARRAY_LIST_MESSAGE circularArrayRemove(HistoryCircularArray* src);

int circularArrayGetCurrent(HistoryCircularArray* src);

#endif