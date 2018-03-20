#ifndef HISTORYCIRCULARARRAY_H
#define HISTORYCIRCULARARRAY_H

#include <stdbool.h>
#include "Enums.h"

typedef struct history_circular_array {
	struct ChessGame** elements;
	int actualSize;
	int maxSize;
	int index;
} HistoryCircularArray;

HistoryCircularArray* circularArrayCreate(int maxSize);

void circularArrayDestroy(HistoryCircularArray* src);

void circularArrayAdd(HistoryCircularArray* src, struct ChessGame* elem);

ARRAY_LIST_MESSAGE circularArrayRemove(HistoryCircularArray* src);

struct ChessGame* circularArrayGetCurrent(HistoryCircularArray* src);

#endif