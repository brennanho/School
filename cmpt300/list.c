#include "list.h"

Node* heads[10];
Node* nodes[10];
int headsIndex = 0; // Keeps track of how many list heads are in use
int nodesIndex = 0; // Keeps track of how many list nodes are in use

LIST* ListCreate() {
	if (headsIndex < 10) {
		LIST* newList = malloc(sizeof *newList);
		newList->size = 0; // Refers to number of elements in the list
		heads[headsIndex++]->inUse = 1;
		newList->first = NULL;
		newList->last = NULL;
		newList->currItem = NULL;
		return newList;
	}
	return NULL; // Failed to create new list as all heads are used up
}

int ListCount(LIST* list) {
	return list->size;
}

void* ListFirst(LIST* list) {
	list->currItem = list->first->item;
	return list->first->item;
}

void* ListLast(LIST* list) {
	list->currItem = list->last->item;
	return list->last->item;
}

// void* ListNext(LIST* list) {
	
// }

// void *ListPrev(LIST* list) {
	
// }

// void *ListCurr(LIST* list) {
	
// }

void AddEmpty(LIST* list, void* item) {
	list->currItem = item;
	nodes[nodesIndex]->inUse = 1;
	list->first = nodes[nodesIndex++];
	list->first->next = NULL;
	
	list->last = list->first;
	list->first->item = item;
	list->last->item = item;
	list->size++;
}

int ListAdd(LIST* list,void* item) {
	if (list->size == 0) {
		AddEmpty(list,item);
		return 0;
	}
	return 0;
}

// int ListInsert(LIST* list,void* item) {
// 	return 0;
// }