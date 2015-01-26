// Author: Brian Rieder
// Title: PA06-IntegerPartitioning-Recursion.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 20
#define TRUE 1
#define FALSE 0

static void printPartition(int * arr, int len)
{
	int i;
	printf("= ");
	for(i = 0; i < len - 1; ++i) 
		printf("%d + ", arr[i]);
	printf("%d\n", arr[len - 1]);
}

static void partitionHelper(int value, int * arr, int ind, 
	int (*testFunction)(int value, int * arr, int ind))
{
	if(value == 0) {
		printPartition(arr, ind);
		return;
	}
	int nextVal;
	for(nextVal = 1; nextVal <= value; ++nextVal) {
		if(testFunction(nextVal, arr, ind)) {
			arr[ind] = nextVal;
			partitionHelper(value - nextVal, arr, ind + 1, testFunction);
		}
	}
}

static int alwaysTrue(int value, int * arr, int ind)
{
	return TRUE;
}

static int isIncreasing(int value, int * arr, int ind)
{
    if(ind == 0) return TRUE;
    return value > arr[ind-1];
}

static int isDecreasing(int value, int * arr, int ind)
{
    if(ind == 0) return TRUE;
    return value < arr[ind-1];
}

static int isOdd(int value, int * arr, int ind)
{
    return value % 2 != 0;
}

static int isEven(int value, int * arr, int ind)
{
    return value % 2 == 0;
}

static int isOddAndEven(int value, int * arr, int ind)
{
    if(ind == 0) return TRUE;
    return (value % 2) != (arr[ind-1] % 2);
}

static int isPrime(int value, int * arr, int ind)
{
	if(value == 1)
		return FALSE;
	if(value == 2)
		return TRUE;
	if(value % 2 == 0)
		return FALSE;
	int factor;
	for(factor = 3; factor < value; factor += 2)
		if(value % factor == 0)
			return FALSE;
	return TRUE;
}

void partitionAll(int value)
{
	int arr[MAX_LEN];
	partitionHelper(value, arr, 0, alwaysTrue);
}

void partitionIncreasing(int value)
{
    int arr[MAX_LEN]; 
    partitionHelper(value, arr, 0, isIncreasing);
}

void partitionDecreasing(int value)
{
    int arr[MAX_LEN]; 
    partitionHelper(value, arr, 0, isDecreasing);
}

void partitionOdd(int value)
{
    int arr[MAX_LEN]; 
    partitionHelper(value, arr, 0, isOdd);
}

void partitionEven(int value)
{
    int arr[MAX_LEN]; 
    partitionHelper(value, arr, 0, isEven);
}

void partitionOddAndEven(int value)
{
    int arr[MAX_LEN]; 
    partitionHelper(value, arr, 0, isOddAndEven);
}

void partitionPrime(int value)
{
    int arr[MAX_LEN]; 
    partitionHelper(value, arr, 0, isPrime);
}
