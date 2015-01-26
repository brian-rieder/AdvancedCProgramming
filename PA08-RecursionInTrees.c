// Author: Brian Rieder
// Title: PA08-RecursionInTrees.c

#include <stdio.h>
#include <stdlib.h>
#include "answer08.h"

SparseNode * SparseNode_create(int index, int value) 
{
    SparseNode * node = malloc(sizeof(SparseNode));
    node->index = index;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

SparseNode * SparseArray_insert(SparseNode * array, int index, int value)
{
	if(value == 0) return array;
	if(array == NULL) return SparseNode_create(index, value);
	if(array->index < index) array->right = SparseArray_insert(array->right, index, value);
	if(array->index > index) array->left = SparseArray_insert(array->left, index, value);
	else array->value = value;
	return array;
}

SparseNode * SparseArray_build(int * indices, int * values, int length)
{
	int i;
	SparseNode * node = NULL;
	for(i = 0; i < length; ++i) 
		node = SparseArray_insert(node, indices[i], values[i]);
	return node;
}

void SparseArray_destroy(SparseNode * array) 
{
    if(array == NULL) return;
    SparseArray_destroy(array->left);
    SparseArray_destroy(array->right);
    free(array);
}

int SparseArray_getMin(SparseNode * array) 
{
    if(array->left == NULL) return array->index;
    else return SparseArray_getMin(array->left);
}

int SparseArray_getMax(SparseNode * array) 
{
    if(array->right == NULL) return array->index;
    else return SparseArray_getMax(array->right);
}

SparseNode * SparseArray_getNode(SparseNode * array, int index) 
{
    if(array == NULL) return NULL;
    if(array->index == index) return array;
    if(array->index < index) return SparseArray_getNode(array->right, index);
    else return SparseArray_getNode(array->left, index);
}

SparseNode * SparseArray_remove(SparseNode * array, int index) 
{
    if(array == NULL) return NULL;
    if(array->index == index) {
        if(array->left == NULL && array->right == NULL) {
            free(array);
            return NULL;
        }
        else if(array->left == NULL) {
            SparseNode * tmp = array->right;
            free(array);
            return tmp;
        }
        else if(array->right == NULL) {
            SparseNode * tmp = array->left;
            free(array);
            return tmp;
        }
        else {
            SparseNode * successor = array->right;
            while(successor->left != NULL)
                successor = successor->left;
            array->index = successor->index;
            array->value = successor->value;
            successor->index = index;
            array->right = SparseArray_remove(array->right, index);
            return array;
        }
    }
    if(array->index < index) {
        array->right = SparseArray_remove(array->right, index);
        return array;
    }
    else {
        array->left = SparseArray_remove(array->left, index);
        return array;
    }
}

SparseNode * SparseArray_copy(SparseNode * array) 
{
    if(array == NULL) return NULL;
    SparseNode * node = SparseNode_create(array->index, array->value);
    node->left = SparseArray_copy(array->left);
    node->right = SparseArray_copy(array->right);
    return node;
}

SparseNode * SparseArray_merge_helper(SparseNode * merged, SparseNode * array_2) 
{
    if(array_2 == NULL) return merged;
    else {
        merged = SparseArray_merge_helper(merged, array_2->left);
        merged = SparseArray_merge_helper(merged, array_2->right);

        SparseNode * snode = SparseArray_getNode(merged, array_2->index);
        if(snode != NULL) {
            snode->value += array_2->value;
            if(snode->value == 0) merged = SparseArray_remove(merged, snode->index);
        }
        else merged = SparseArray_insert(merged, array_2->index, array_2->value);
        return merged;
    }
}

SparseNode * SparseArray_merge(SparseNode * array_1, SparseNode * array_2) {
    SparseNode * merged = SparseArray_copy(array_1);
    return SparseArray_merge_helper(merged, array_2);
}
