#include "list.h"

Node* heads[10];
Node* nodes[100];
int headsIndex = 0; // Keeps track of how many list heads are in use
int nodesIndex = 0; // Keeps track of how many list nodes are in use

LIST* ListCreate() {
	if (headsIndex < 10) {
		LIST* newList = malloc(sizeof *newList);
		newList->size = 0; // Refers to number of elements in the list
		newList->first = NULL;
		newList->last = NULL;
		heads[headsIndex++]->inUse = 1;
		int a = 5;
		nodes[0]->item = &a;
		newList->first = nodes[0];
		newList->last = nodes[0];
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

// //returns a pointer to the current item in list.
// void *ListCurr(LIST* list) {
	
// }

// int ListAdd(LIST* list,void* item) {

// 	return 0;
// }