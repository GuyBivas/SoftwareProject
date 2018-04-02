#ifndef HISTORYCIRCULARARRAY_H
#define HISTORYCIRCULARARRAY_H

#include <stdbool.h>
#include "ChessGame.h"
#include "Enums.h"

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

ChessGame* circularArrayGetCurrent(HistoryCircularArray* src);

bool circularArrayListIsEmpty(HistoryCircularArray* src);

int circularArrayListSize(HistoryCircularArray* src);


#endif