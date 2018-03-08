#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

void Replace(unsigned int A[], int size, unsigned int(*func)(unsigned int))
{
	for (int i = 0; i < size; i++)
	{
		A[i] = (*func)(A[i]);
	}
}

unsigned int ReplaceNum(unsigned int num) 
{
	int a, b, c, d;
	int maskD = (1 << 8) - 1;
	int maskC = maskD << 8;
	int maskB = maskC << 8;
	int maskA = maskB << 8;
	
	a = (maskA & num) >> 24;
	b = (maskB & num) >> 16;
	c = (maskC & num) >> 8;
	d = (maskD & num);

	return (b << 24) | (a << 16) | (d << 8) | c;
}

void RepCng(unsigned int A[], int size)
{
	unsigned int(*func)(unsigned int) = &ReplaceNum;
	Replace(A, size, func);
}

char * int2bin(int i)
{
	size_t bits = sizeof(int) * CHAR_BIT;

	char * str = (char *)malloc(bits + 1);
	if (!str) return NULL;
	str[bits] = 0;

	// type punning because signed shift is implementation-defined
	unsigned u = *(unsigned *)&i;
	for (; bits--; u >>= 1)
		str[bits] = u & 1 ? '1' : '0';

	printf("%s\n", str);
	return str;
}

int main()
{
	unsigned int A[] = { 4457645678 };
	int2bin(A[0]);
	RepCng(A, 1);
	int2bin(A[0]);
	//printf("%d", A[0]);
	getchar();
}

void* binSearch(void* arr, int size, int elemSize, void* item, int(*compare)(void*, void*))
{
	int compRes = 0;
	int place = 0;
	//int i = (int)arr[0];
	compRes = (*compare)(item, (char*)arr + place*elemSize);
	//compRes = (*compare)(item, &(arr[place*elemSize]));
}