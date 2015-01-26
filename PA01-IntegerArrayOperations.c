// Author: Brian Rieder
// Title: PA01-IntegerArrayOperations.c

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

int arraySum(int * array, int len)
{
	int i, sum = 0;
	for(i = 0; i < len; ++i) 
		sum += array[i];
	return sum;
}

int arrayCountNegative(int * array, int len)
{
	int i, count = 0;
	for(i = 0; i < len; ++i)
		if(array[i] < 0)
			++count;
	return count;
}

int arrayIsIncreasing(int * array, int len)
{
	int i;
	for(i = 1; i < len; ++i)
		if(array[i] < array[i - 1])
			return FALSE;
	return TRUE;
}

int arrayIndexRFind(int needle, const int * haystack, int len)
{
	int i;
	for(i = len - 1; i >= 0; --i)
		if(haystack[i] == needle)
			return i;
	return -1;
}

int arrayFindSmallest(int * array, int len)
{
	int i, min_index = 0;
	for(i = 1; i < len; ++i)
		if(array[i] < array[min_index])
			min_index = i;
	return min_index;
}
