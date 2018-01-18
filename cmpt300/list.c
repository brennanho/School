#include "list.h"

Head* heads[10];
Node* nodes[10];
int headsIndex = 0; // Keeps track of how many list heads are in use
int nodesIndex = 0; // Keeps track of how many list nodes are in use

LIST* ListCreate() {
	if (headsIndex < 10) {
		LIST* newList = malloc(sizeof *newList);
		newList->size = 0; // Refers to number of elements in the list
		newList->head = heads[headsIndex++];
		newList->head->first = newList->first; //Make the head point to the first item node in the list
		newList->first = NULL;
		newList->last = NULL;
		newList->curr = NULL;
		return newList;
	}
	return NULL; // Failed to create new list as all heads are used up
}

// Print content of list (set to int right now)
void PrintList(LIST* list) {
	Node* currNode = list->first;
	while (currNode != NULL) {
		printf("%d->",*(int*)currNode->item);
		currNode = currNode->next;
	}
	printf("\n");
}

// Adds the first node into the list
void AddToEmptyList(LIST* list, void* item) {
	list->first = nodes[nodesIndex++];
	list->first->next = NULL;
	list->last = list->first;
	list->curr = list->first;
	list->first->item = item;
	list->last->item = item;
	list->curr->item = item;
	list->size++;
}

int ListCount(LIST* list) {
	return list->size;
}

void* ListFirst(LIST* list) {
	list->curr = list->first;
	list->curr->item = list->first->item;
	return list->first;
}

void* ListLast(LIST* list) {
	list->curr = list->last;
	list->curr->item = list->last->item;
	return list->last;
}

void* ListNext(LIST* list) {
	if (list->curr->next != NULL) {
		list->curr = list->curr->next;
		return list->curr;
	}
	return NULL;
}

// void *ListPrev(LIST* list) {
	
// }

// void *ListCurr(LIST* list) {
	
// }

int ListAdd(LIST* list,void* item) {
	if (list->size == 0) {
		AddToEmptyList(list,item);
		return 0;
	} else if (list->curr != NULL) {
		list->curr->next = nodes[nodesIndex++];
		list->curr = list->curr->next;
		list->curr->item = item;
		list->curr->next = NULL;
		list->size++;
	}
 	return 0;
}

// int ListInsert(LIST* list,void* item) {
// 	return 0;
// }