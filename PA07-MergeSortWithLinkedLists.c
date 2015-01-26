// Author: Brian Rieder
// Title: PA07-MergeSortWithLinkedLists.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

List * List_createNode(const char * str)
{
	List * list = malloc(sizeof(List));
	list->str = strdup(str);
	list->next = NULL;
	return list;
}

void List_destroy(List * list)
{
	if(list != NULL) {
		List_destroy(list->next);
		free(list->str);
		free(list);
	}
}

int List_length(List * list)
{
	return (list == NULL) ? 0 : (1 + List_length(list->next));
}

List * List_merge(List * lhs, List * rhs, int(*compar)(const char *, const char *))
{
	if(lhs == NULL) return rhs;
	if(rhs == NULL) return lhs;
	List * head = NULL;
	List * tail = NULL;
	while(lhs != NULL && rhs != NULL) {
		List * * smallest = (compar(lhs->str, rhs->str) <= 0) ? &lhs : &rhs;
		if(tail == NULL) {
			head = *smallest;
			tail = head;
		}
		else {
			tail->next = *smallest;
			tail = tail->next;
		}
		*smallest = (*smallest)->next;
		tail->next = NULL;
	}
	tail->next = (lhs == NULL) ? rhs : lhs;
	return head;
}

List * List_sort(List * list, int (*compar)(const char *, const char *))
{
	int len = List_length(list);
	if(len <= 1) return list;
	int len_halved = len / 2;
	List * lhs = list;
	List * lhs_tail = lhs;
	while(--len_halved > 0)
		lhs_tail = lhs_tail->next;
	lhs = List_sort(lhs, compar);
	rhs = List_sort(rhs, compar);
	return List_merge(lhs, rhs, compar);
}
