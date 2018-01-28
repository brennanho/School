#include<stdio.h>
#include <stdlib.h>
#include "list.h"

LIST heads[headsArrSize];
Node nodes[nodesArrSize];
LIST headsStackMem = {NULL,NULL,NULL,0,0};
LIST nodesStackMem = {NULL,NULL,NULL,0,0};
LIST* headsStack = &headsStackMem;
LIST* nodesStack = &nodesStackMem;
int headsIndex = 0; // Keeps track of how many list heads are in use
int nodesIndex = 0; // Keeps track of how many list nodes are in use

//*----Helper Functions----*/

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

Node* getAvailableNode(int resource) { 
	if (resource == 0) {
		if (nodesIndex < nodesArrSize) {
			return &nodes[nodesIndex++];
		} else if (nodesStack->size > 0) {
			printf("Getting from nodesStack\n");
			Node* newNode = nodesStack->last;
			nodesStack->last = nodesStack->last->prev;
			nodesStack->size--;
			return newNode;
		}
	}
	return NULL;
}

// Adds the first node into the list
// This is only called by the other "adding to list" functions when the list is empty
void AddToEmptyList(LIST* list, void* item) {
	list->first = getAvailableNode(0);
	list->first->item = item;
	list->first->next = NULL;
	list->first->prev = NULL;
	list->last = list->first;
	list->curr = list->first;
	list->size++;
}

//comparator routine used for ListSearch (must be typecasted, used int for the case of this assignment)
int comparator(void* item, void* comparisonArg) {
	if (*(int*)item == *(int*)comparisonArg)
		return 1;
	return 0;
}

void StackAppend(LIST* stack, void* node) {
	if (stack->first == 0) {
		stack->first = node;
		stack->last = node;
	} else {
		Node* tempLast = stack->last;
		stack->last = stack->last->next;
		stack->last = node;
		stack->last->prev = tempLast;
	}
	stack->size++;
}

//*----End of Helper Functions----*//

LIST* ListCreate() {
	if (headsIndex < headsArrSize) {
		LIST* newList = &heads[headsIndex++];
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
			list->outOfBounds = 1;
		return list->curr;
	}
	else if (list->outOfBounds == -1) 
		list->curr = list->first;
	else if (list->outOfBounds == 1)
		list->curr = list->last;
	
	list->outOfBounds = 0;
	return list->curr;
}

void *ListPrev(LIST* list) {
	if (list->curr != NULL) {
		list->curr = list->curr->prev;
		if (list->curr == NULL)
			list->outOfBounds = -1;
		return list->curr;
	} 
	else if (list->outOfBounds == -1) 
		list->curr = list->first;
	else if (list->outOfBounds == 1)
		list->curr = list->last;
	
	list->outOfBounds = 0;
	return list->curr;
}


void *ListCurr(LIST* list) {
	return list->curr;
}

int ListAdd(LIST* list,void* item) {
	if (nodesIndex < nodesArrSize || nodesStack->size > 0) {
		if (list->size == 0) 
			AddToEmptyList(list,item);
		else if (list->outOfBounds == 1 || list->last == list->curr) //current pointer is the last item in the list
			ListAppend(list,item);
		else if (list->outOfBounds == -1 || list->curr == NULL) //current pointer is beyond the start of the list
			ListPrepend(list,item);
		else if (list->curr != NULL) { //current pointer is contained within the list
			Node* newNode = &nodes[nodesIndex++];
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
	if (nodesIndex < nodesArrSize || nodesStack->size > 0) {
		if (list->size == 0) 
			AddToEmptyList(list,item);
		else if (list->outOfBounds == -1 || list->first == list->curr) //current pointer is beyond the start of the list
			ListPrepend(list,item);
		else if (list->outOfBounds == 1 || list->curr == NULL) //current pointer is beyond the end of the list
			ListAppend(list,item);
		else if (list->curr != NULL) { //current pointer is contained within the list
			Node* newNode = &nodes[nodesIndex++];
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
	if (nodesIndex < nodesArrSize || nodesStack->size > 0) {
		if (list->size == 0) {
			AddToEmptyList(list,item);
		} else {
			Node* lastPrev = list->last;
			list->last->next = getAvailableNode(0);
			list->last = list->last->next;
			list->last->prev = lastPrev;
			list->last->next = NULL;
			list->last->item = item;
			list->curr = list->last;
			list->size++;
		}
		return 0;
	}
	return -1;
}

int ListPrepend(LIST* list, void* item) {
	if (nodesIndex < nodesArrSize || nodesStack->size > 0) {
		if (list->size == 0) 
			AddToEmptyList(list,item);
		else {
			Node* firstPrev = list->first;
			list->first->prev = getAvailableNode(0);
			list->first = list->first->prev;
			list->first->next = firstPrev;
			list->first->prev = NULL;
			list->first->item = item; 
			list->curr = list->first;
			list->size++;
		}
		return 0;
	}
	return -1;
}

void *ListRemove(LIST* list) {
	if (list->size > 0 && list->curr != NULL) {
		Node* toRemove = list->curr;
		if (list->curr == list->last || list->curr == list->last) {
			ListTrim(list);
		} else if (list->curr->next != NULL) {
			list->curr->prev->next = list->curr->next;
			list->curr->next->prev = list->curr->prev;
			list->curr = list->curr->next;
			list->size--;
			StackAppend(nodesStack,toRemove);
		}

		//printf("%d\n",list->size);


		//PrintList(nodesStack);

		return toRemove;
	}
	return NULL;
}

void *ListTrim(LIST* list) {
	if (list->size > 0) {
		Node* toTrim = list->last;
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
		StackAppend(nodesStack,toTrim);
		return toTrim->item;
	}
	return NULL;
}

void ListConcat(LIST* list1,LIST* list2) {
	if (list1->size > 0) {
		list1->last->next = list2->first;
		list1->size = list1->size + list2->size;
		//heads[list2->index] = NULL;
	}
}

void *ListSearch(LIST* list, int (*comparator)(void*,void*),void* comparisonArg) {
	Node* curr = list->curr;
	while (curr != NULL) {
		if (comparator(comparisonArg,curr->item) == 1)
		{
			list->curr = curr;
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}