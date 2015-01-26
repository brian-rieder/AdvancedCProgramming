// Author: Brian Rieder
// Title: PA09-HuffmanDecoding.c

#include <stdio.h>
#include <stdlib.h>
#include "answer09.h"

#define FATAL(msg) { 													\
	fprintf(stderr, "FATAL %s:%d %s\n", __FILE__, (int) __LINE__, msg); \
	exit(1); 															\
}

#define TRUE 1
#define FALSE 0

HuffNode * HuffNode_create(int value)
{
    HuffNode * node = malloc(sizeof(HuffNode));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void HuffNode_destroy(HuffNode * node)
{
    if(node == NULL) return;
    HuffNode_destroy(node->left);
    HuffNode_destroy(node->right);
    free(node);
}

Stack * Stack_create()
{
    return calloc(1, sizeof(Stack));
}

void Stack_destroy(Stack * stack)
{
    if(stack == NULL) return;
    StackNode * itr = stack->head;
    while(itr != NULL) {
		StackNode * tmp = itr;
		itr = itr->next;
		HuffNode_destroy(tmp->tree);
		free(tmp);
    }
    free(stack);
}

int Stack_isEmpty(Stack * stack)
{
    return stack->head == NULL;
}

HuffNode * Stack_popFront(Stack * stack)
{
    if(Stack_isEmpty(stack)) FATAL("Stack underflow!");
    HuffNode * ret = stack->head->tree;
    StackNode * tmp = stack->head;
    stack->head = stack->head->next;
    free(tmp);
    return ret;
}

void Stack_pushFront(Stack * stack, HuffNode * tree)
{
    StackNode * head = malloc(sizeof(StackNode));
    head->tree = tree;
    head->next = stack->head;
    stack->head = head;
}

void Stack_popPopCombinePush(Stack * stack)
{
    HuffNode * first_leaf = Stack_popFront(stack);
    HuffNode * second_leaf = Stack_popFront(stack);
    HuffNode * new_tree = HuffNode_create(first_leaf->value + second_leaf->value);
    new_tree->left = second_leaf;
    new_tree->right = first_leaf;
    Stack_pushFront(stack, new_tree);
}

typedef struct BitFile_st
{
    FILE * fp;
    unsigned char byte; // Current byte being processed
    int offset;         // offset into current byte gives the current bit
} BitFile;

BitFile * BitFile_create(FILE * fp)
{
    BitFile * bp = malloc(sizeof(BitFile));
    bp->fp = fp;
    bp->byte = 0;
    bp->offset = 8; // The next read will trigger init of byte
    return bp;
}

void BitFile_destroy(BitFile * bp)
{
    if(bp != NULL)
	free(bp);
}

int BitFile_nextBit(BitFile * bf)
{
    if(bf->offset == 8) {
	bf->offset = 0;
	if(fread(&(bf->byte), sizeof(unsigned char), 1, bf->fp) != 1) return -1;
    return ((bf->byte) >> (7 - (bf->offset++))) & 0x01;
}

int BitFile_nextByte(BitFile * bf)
{
    int ret = 0;
    int offset;
    for(offset = 0; offset < 8; ++offset) {
		int bit = BitFile_nextBit(bf);
		if(bit < 0) return -1;
		ret = ret | (bit << (7-offset));
    }
    return ret;
}

HuffNode * HuffTree_readTextHeader(FILE * fp)
{
    Stack * stack = Stack_create();
    int read_char = fgetc(fp);
    while(!feof(fp)) {
    	if(read_char == '1') {
    		read_char = fgetc(fp);
    		if(feof(fp)) FATAL("Incomplete header");
    		Stack_pushFront(stack, HuffNode_create(read_char));
    	} else if(read_char == '0') {
    		if(stack->head != NULL && stack->head->next == NULL) break;
    		Stack_popPopCombinePush(stack);
    	}
    	read_char = fgetc(fp);
    }

    if(Stack_isEmpty(stack)) FATAL("Empty or corrupt header");    
    HuffNode * tree = Stack_popFront(stack);
    if(!Stack_isEmpty(stack)) FATAL("Tree is not empty after popping 'last' node!");
    Stack_destroy(stack);
    return tree;
}

HuffNode * HuffTree_readBinaryHeader(FILE * fp)
{
    Stack * stack = Stack_create();
    BitFile * bit_file = BitFile_create(fp);
    int read_bit = BitFile_nextBit(bit_file);
    while(read_bit >= 0) {
    	if(read_bit == 1) {
    		read_bit = BitFile_nextByte(bit_file);
    		if(read_bit < 0) FATAL("Incomplete header");
    		Stack_pushFront(stack, HuffNode_create(read_bit));
    	} else if(read_bit == 0) {
    		if(stack->head != NULL && stack->head->next == NULL) break;
    		Stack_popPopCombinePush(stack);
    	}
    	read_bit = BitFile_nextBit(bit_file);
    }

    if(Stack_isEmpty(stack)) FATAL("Empty or corrupt header");    
    HuffNode * tree = Stack_popFront(stack);
    if(!Stack_isEmpty(stack)) FATAL("Tree is not empty after popping 'last' node!");
    Stack_destroy(stack);
    BitFile_destroy(bit_file);
    return tree;
}
