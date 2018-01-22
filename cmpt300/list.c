#include "list.h"
#define headsArrSize 10
#define nodesArrSize 10

LIST* heads[headsArrSize];
Node* nodes[nodesArrSize];
int headsIndex = 0; // Keeps track of how many list heads are in use
int nodesIndex = 0; // Keeps track of how many list nodes are in use

//*----Helper Functions----*//

// Print content of list (set to int right now)
void PrintList(LIST* list) {
	Node* currNode = list->first;
	if (list->size > 0) {
		while (currNode->next != NULL) {
			if (currNode == list->curr)
				printf("%d(curr)->",*(int*)currNode->item);
			else
				printf("%d->",*(int*)currNode->item);
			currNode = currNode->next;
		}
		if (currNode == list->curr)
			printf("%d(curr)\n",*(int*)currNode->item);
		else
			printf("%d\n",*(int*)currNode->item);
	}
	else {
		printf("Empty List\n");
	}
}

// Adds the first node into the list
// This is only called by the other "adding to list" functions when the list is empty
void AddToEmptyList(LIST* list, void* item) {
	list->first = nodes[nodesIndex++];
	list->first->item = item;
	list->first->next = NULL;
	list->first->prev = NULL;
	list->last = list->first;
	list->curr = list->first;
	list->size++;
}

//*----End of Helper Functions----*//

LIST* ListCreate() {
	if (headsIndex < headsArrSize) {
		LIST* newList = heads[headsIndex++];
		newList->size = 0; // Refers to number of elements in the list
		newList->outOfBounds = 0;
		newList->first = NULL;
		newList->last = NULL;
		newList->curr = NULL;
		return newList;
	}
	return NULL; // Failed to create new list as all heads are used up
}

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
	if (nodesIndex < nodesArrSize) {
		if (list->size == 0) 
			AddToEmptyList(list,item);
		else if (list->outOfBounds > 0 || list->last == list->curr) //current pointer is the last item in the list
			ListAppend(list,item);
		else if (list->outOfBounds < 0 || list->curr == NULL) //current pointer is beyond the start of the list
			ListPrepend(list,item);
		else if (list->curr != NULL) { //current pointer is contained within the list
			Node* newNode = nodes[nodesIndex++];
			newNode->item = item;
			
			list->curr->next->prev = newNode;
			newNode->next = list->curr->next;

			list->curr->next = newNode;
			newNode->prev = list->curr;

			list->curr = newNode;
			list->size++;
		} 

		return 0;
	}
 	return -1;
}

int ListInsert(LIST* list,void* item) {
	if (nodesIndex < nodesArrSize) {
		if (list->size == 0) 
			AddToEmptyList(list,item);
		else if (list->outOfBounds < 0 || list->first == list->curr) //current pointer is beyond the start of the list
			ListPrepend(list,item);
		else if (list->outOfBounds > 0 || list->curr == NULL) //current pointer is beyond the end of the list
			ListAppend(list,item);
		else if (list->curr != NULL) { //current pointer is contained within the list
			Node* newNode = nodes[nodesIndex++];
			newNode->item = item;

			list->curr->prev->next = newNode;
			newNode->prev = list->curr->prev;

			newNode->next = list->curr;
			list->curr->prev = newNode;

			list->curr = newNode;
			list->size++;
		}
		return 0;
	}
 	return -1;
}

int ListAppend(LIST* list,void* item) {
	if (nodesIndex < nodesArrSize) {
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
			list->size++;
		}
		
		return 0;
	}
	return -1;
}

int ListPrepend(LIST* list, void* item) {
	if (nodesIndex < nodesArrSize) {
		if (list->size == 0) 
			AddToEmptyList(list,item);
		else {
			Node* firstPrev = list->first;
			list->first->prev = nodes[nodesIndex++];
			list->first = list->first->prev;
			list->first->next = firstPrev;
			list->first->prev = NULL;
			list->first->item = item; 
			list->curr = list->first;

			if (list->outOfBounds < 0)
				list->outOfBounds++;
			list->size++;
		}
		return 0;
	}
	return -1;
}

void *ListRemove(LIST* list) {
	if (list->size > 0 && list->curr != NULL) {
		Node* toRemove = list->curr;
		if (list->curr == list->last) {
			list->curr->prev->next = NULL;
			list->curr = list->curr->prev;
			list->last = list->curr;
		} else if (list->curr == list->first) {
			list->curr->next->prev = NULL;
			list->curr = list->curr->next;
			list->first = list->curr;
		} else if (list->curr->next != NULL) {
			list->curr->prev->next = list->curr->next;
			list->curr->next->prev = list->curr->prev;
			list->curr = list->curr->next;
		}
		nodes[--nodesIndex] = toRemove;
		list->size--;
	}
}

void *ListTrim(LIST* list) {
	if (list->size > 0) {
		void* toTrim = list->last->item;
		if (list->size == 1) {
			list->first = NULL;
			list->last = NULL;
			list->curr = NULL;
		} else if (list->last != NULL) {
			list->last = list->last->prev;
			list->last->next = NULL;
			list->curr = list->last;
		}
		list->size--;
		return toTrim;
	}
	return NULL;
}