#include "list.h"

Head* heads[10];
Node* nodes[10];
int headsIndex = 0; // Keeps track of how many list heads are in use
int nodesIndex = 0; // Keeps track of how many list nodes are in use

LIST* ListCreate() {
	if (headsIndex < 10) {
		LIST* newList = malloc(sizeof *newList);
		newList->size = 0; // Refers to number of elements in the list
		newList->outOfBounds = 0;
		newList->head = heads[headsIndex++];
		newList->head->first = newList->first; //Make the head point to the first item node in the list
		newList->first = NULL;
		newList->last = NULL;
		newList->curr = NULL;
		return newList;
	}
	return NULL; // Failed to create new list as all heads are used up
}

//*----Helper Functions----*//

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
	list->first->prev = NULL;
	list->last = list->first;
	list->curr = list->first;
	list->first->item = item;
	list->last->item = item;
	list->curr->item = item;
	list->head->first = list->first;
}

//Updates the tail/last node to the end of the list
void UpdateLast(LIST* list) {
	Node* curr = list->first;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	list->last = curr;
	list->last->item = curr->item;
}

//*----End of Helper Functions----*//

int ListCount(LIST* list) {
	return list->size;
}

void* ListFirst(LIST* list) {
	list->curr = list->first;
	return list->first;
}

void* ListLast(LIST* list) {
	list->curr = list->last;
	return list->last;
}

void* ListNext(LIST* list) {
	if (list->curr != NULL) {
		list->curr = list->curr->next;
		if (list->curr == NULL)
			list->outOfBounds++;
		return list->curr;
	} else 
		list->outOfBounds++;

	 if (list->outOfBounds == 0) {
		list->curr = list->first;
		return list->curr;
	}
	return NULL;
}
void *ListPrev(LIST* list) {
	if (list->curr != NULL) {
		list->curr = list->curr->prev;
		if (list->curr == NULL)
			list->outOfBounds--;
		return list->curr;
	} else
		list->outOfBounds--;

	if (list->outOfBounds == 0) {
		list->curr = list->last;
		return list->curr;
	}
	return NULL;
}


void *ListCurr(LIST* list) {
	return list->curr;
}

int ListAdd(LIST* list,void* item) {
	if (nodesIndex < 10) {
		if (list->size == 0) {
			AddToEmptyList(list,item);
		} else if (list->curr != NULL) {
			Node* newNode = nodes[nodesIndex++];
			Node* newPrev = list->curr;
			Node* newNext = list->curr->next;

			newNode->item = item;
			newNode->prev = list->curr->prev;
			newNode->next = list->curr;
			newPrev->next = newNode;
			newNext->prev = newNode;

			list->curr = newNode;
			list->curr->prev = newPrev;
			list->curr->next = newNext;

			list->size++;
		} else if (list->outOfBounds < 0) //Beyond the start of the list
			ListPrepend(list,item);
		  else if (list->outOfBounds > 0) //Beyond the end of the list
		  	ListAppend(list,item);

		list->size++;
		return 0;
	}
 	return -1;
}

int ListInsert(LIST* list,void* item) {
	if (nodesIndex < 10) {
		if (list->size == 0) {
			AddToEmptyList(list,item);
		} else if (list->curr != NULL) {
			Node* newNode = nodes[nodesIndex++];
			Node* newPrev = list->curr->prev;
			Node* newNext = list->curr;

			newNode->item = item;
			newNode->prev = list->curr->prev;
			newNode->next = list->curr;
			newPrev->next = newNode;
			newNext->prev = newNode;

			list->curr = newNode;
			list->curr->prev = newPrev;
			list->curr->next = newNext;

			list->size++;
		} else if (list->outOfBounds < 0) //Beyond the start of the list
			ListPrepend(list,item);
		  else if (list->outOfBounds > 0) //Beyond the end of the list
		  	ListAppend(list,item);

		list->size++;
		return 0;
	}
 	return -1;
}

int ListAppend(LIST* list,void* item) {
	if (nodesIndex < 10) {
		if (list->size == 0) {
			AddToEmptyList(list,item);
		} else {
			Node* lastPrev = list->last;
			list->last->next = nodes[nodesIndex++];
			list->last = list->last->next;
			list->last->prev = lastPrev;
			list->last->next = NULL;
			list->last->item = item;
			list->curr = list->last;
			if (list->outOfBounds > 0)
				list->outOfBounds--;

		}
		list->size++;
		return 0;
	}
	return -1;
}

int ListPrepend(LIST* list, void* item) {
	if (nodesIndex < 10) {
		if (list->size == 0) {
			AddToEmptyList(list,item);
		} else {
			Node* firstPrev = list->first;
			list->first->prev = nodes[nodesIndex++];
			list->first = list->first->prev;
			list->first->next = firstPrev;
			list->first->prev = NULL;
			list->first->item = item; 
			list->curr = list->first;
			list->head->first = list->first;

			if (list->outOfBounds < 0)
				list->outOfBounds++;
		}
		list->size++;
		return 0;
	}
	return -1;
}