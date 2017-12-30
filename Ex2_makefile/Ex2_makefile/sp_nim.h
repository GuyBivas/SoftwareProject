#pragma once
#include <stdbool.h>
int SnimCalculate(int* heapSizes, int n);
void ComputerMove(int *heapSizes, int n);
void PlayerMove(int* heapSizes, int n);
bool CheckWin(int *heapSizes, int n);