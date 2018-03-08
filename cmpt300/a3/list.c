#include<stdio.h>
#include <stdlib.h>
#include "list.h"
#include "process.h"

/*Nodes and Heads Description:
	In this program, nodes and list heads are statically allocated in two arrays.
	When a node or head is removed from the users list, it is pushed onto their
	respective stacks for future use. 
*/

//global variable declaration
LIST heads[headsArrSize]; //Pool of LIST head resources
Node headsRecycle[headsArrSize]; //Used to hold used LIST heads when returned to pool
Node nodes[nodesArrSize]; //Pool of Node resources
LIST headsStackMem = {NULL,NULL,NULL,0,0};
LIST nodesStackMem = {NULL,NULL,NULL,0,0};
LIST* headsStack = &headsStackMem; // Pool of heads stored in a stack (list)
LIST* nodesStack = &nodesStackMem; // Pool of nodes stored in a stack (list)
int headsIndex = 0; // Keeps track of how many list heads are in use from static array
int nodesIndex = 0; // Keeps track of how many list nodes are in use from static array

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

//Retrieves an available node. Either looks to see if there are still unused nodes avaialbe from static array. 
//If no nodes are available in the static array, it looks into the node stack.
//If there are no nodes avaiable, return a NULL pointer
Node* getAvailableNode(void) { 
	if (nodesIndex < nodesArrSize) {
		return &nodes[nodesIndex++];
	} else if (nodesStack->size > 0) {
		Node* newNode = nodesStack->last;
		nodesStack->last = nodesStack->last->prev;
		nodesStack->size--;
		return newNode;
	}
	return NULL;
}

// Adds the first node into the list
// This is only called by the other "adding to list" functions when the list is empty
void AddToEmptyList(LIST* list, void* item) {
	list->first = getAvailableNode();
	list->first->item = item;
	list->first->next = NULL;
	list->first->prev = NULL;
	list->last = list->first;
	list->curr = list->first;
	list->size++;
}

//comparator routine used for ListSearch (must be typecasted, used int for the case of this assignment)
int comparator(void* item, void* comparisonArg) {
	PCB* myItem = (PCB*)item;
	int id = myItem->id;
	if (id == *(int*)comparisonArg)
		return 1;
	return 0;
}

int comparator2(void* item, void* comparisonArg) {
	Message* myItem = (Message*)item;
	int id = myItem->toID;
	if (id == *(int*)comparisonArg)
		return 1;
	return 0;
}

//Returns used nodes to node stack pool 
void itemFree(void* itemToBeFreed) {
	if (nodesStack->size == 0) {
		nodesStack->first = itemToBeFreed;
		nodesStack->last = itemToBeFreed;
	} else {
		Node* tempLast = nodesStack->last;
		nodesStack->last->next = itemToBeFreed;
		nodesStack->last = nodesStack->last->next;
		nodesStack->last->prev = tempLast;
	}
	nodesStack->size++;
}

//Returns used heads to head stack pool
void headFree(LIST* headToBeFreed) {
	headToBeFreed->size = 0;
	headsStack->curr->item = headToBeFreed;
	headsStack->size++;
	ListNext(headsStack);
}

//Stack used to keep number of recycled lists 
void createHeadsStack(void) {
	headsStack->first = &headsRecycle[0];
	headsStack->last = &headsRecycle[0];

	for (int i = 1; i < headsArrSize; i++) {
		Node* tempPrev = headsStack->last;
		headsStack->last->next = &headsRecycle[i];
		headsStack->last = headsStack->last->next;
		headsStack->last->prev = tempPrev;
		headsStack->size++;
	} 
	ListFirst(headsStack);
}

//*----End of Helper Functions----*//

//*----list.h implementation----*//

LIST* ListCreate() {
	if (headsIndex < headsArrSize) {
		if (headsIndex == 0) { // Initialize headStack for "to be returned" list heads
			createHeadsStack();
		}

		LIST* newList = &heads[headsIndex++];
		newList->size = 0;
		newList->outOfBounds = 0;
		newList->first = NULL;
		newList->last = NULL;
		newList->curr = NULL;
		return newList;
	} else if (headsStack->size > 0) {
		ListPrev(headsStack);
		LIST* newList = headsStack->curr->item;
		headsStack->size--;
		return newList;
	}
	return NULL; // Failed to create new list as all heads are used up
}

int ListCount(LIST* list) {
	return list->size;
}

void* ListFirst(LIST* list) {
	list->curr = list->first;
	if (list->first != NULL)
		return list->first->item;
	return NULL;
}

void* ListLast(LIST* list) {
	list->curr = list->last;
	if (list->last != NULL)
		return list->last->item;
	return NULL;
}

void* ListNext(LIST* list) {
	if (list->curr != NULL) {
		list->curr = list->curr->next;
		if (list->curr == NULL) {
			list->outOfBounds = 1;
			return NULL;
		}
		return list->curr->item;
	}
	else if (list->outOfBounds == -1) 
		list->curr = list->first;
	else if (list->outOfBounds == 1)
		list->curr = list->last;
	
	list->outOfBounds = 0;
	if (list->curr != NULL)
		return list->curr->item;
	return NULL;
}

void *ListPrev(LIST* list) {
	if (list->curr != NULL) {
		list->curr = list->curr->prev;
		if (list->curr == NULL){
			list->outOfBounds = -1;
			return NULL;
		}
		return list->curr->item;
	} 
	else if (list->outOfBounds == -1) 
		list->curr = list->first;
	else if (list->outOfBounds == 1)
		list->curr = list->last;
	
	list->outOfBounds = 0;
	if (list->curr != NULL)
		return list->curr->item;
	return NULL;
}


void *ListCurr(LIST* list) {
	if (list->curr != NULL) {
		return list->curr->item;
	}
	return NULL; // current item is beyond one end of the list or list->size == 0
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
			Node* newNode = getAvailableNode();
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
	printf("Error: no more available nodes\n");
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
			Node* newNode = getAvailableNode();
			newNode->item = item;

			list->curr->prev->next = newNode;
			newNode->prev = list->curr->prev;

			list->curr->prev = newNode;
			newNode->next = list->curr;

			list->curr = newNode;
			list->size++;
		}
		return 0;
	}
	printf("Error: no more available nodes\n");
 	return -1;
}

int ListAppend(LIST* list,void* item) {
	if (nodesIndex < nodesArrSize || nodesStack->size > 0) {
		if (list->size == 0) {
			AddToEmptyList(list,item);
		} else {
			Node* lastPrev = list->last;
			list->last->next = getAvailableNode();
			list->last = list->last->next;
			list->last->prev = lastPrev;
			list->last->next = NULL;
			list->last->item = item;
			list->curr = list->last;
			list->size++;
		}
		return 0;
	}
	printf("Error: no more available nodes\n");
	return -1;
}

int ListPrepend(LIST* list, void* item) {
	if (nodesIndex < nodesArrSize || nodesStack->size > 0) {
		if (list->size == 0) 
			AddToEmptyList(list,item);
		else {
			Node* firstPrev = list->first;
			list->first->prev = getAvailableNode();
			list->first = list->first->prev;
			list->first->next = firstPrev;
			list->first->prev = NULL;
			list->first->item = item; 
			list->curr = list->first;
			list->size++;
		}
		return 0;
	}
	printf("Error: no more available nodes\n");
	return -1;
}

void *ListRemove(LIST* list) {
	if (list->size > 0 && list->curr != NULL) {
		Node* toRemove = list->curr;
		if (list->curr == list->last || list->size == 1) {
			ListTrim(list); 
		} else if (list->curr == list->first) { 
			list->first = list->first->next;
			list->first->prev = NULL;
			list->curr = list->first;
			list->size--;
			itemFree(toRemove);
		} else if (list->curr->next != NULL) {
			list->curr->prev->next = list->curr->next;
			list->curr->next->prev = list->curr->prev;
			list->curr = list->curr->next;
			list->size--;
			itemFree(toRemove);
		}
		return toRemove->item;
	}
	return NULL; //List is empty, no items to remove
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
		itemFree(toTrim);
		return toTrim->item;
	}
	return NULL; //List is empty, no items to trim
}

void ListFree(LIST* list,void (*itemFree)(void* itemToBeFreed)) {
	while (list->size > 0) {
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
		itemFree(toTrim);
	} 

	headFree(list);
}

void ListConcat(LIST* list1,LIST* list2) {
	if (list1->size > 0) {
		Node* curr = list1->curr;
		ListFirst(list2);
		while (list2->size > 0) {
			ListAppend(list1,ListRemove(list2));
		}
		list1->curr = curr;
		headFree(list2); //List head is now put back into resource pool while nodes are still in use
	}
}

void *ListSearch(LIST* list, int (*comparator)(void*,void*),void* comparisonArg) {
	Node* curr = list->first;
	while (curr != NULL) {
		if (comparator(comparisonArg,curr->item) == 1)
		{
			list->curr = curr;
			return list->curr->item;
		}
		curr = curr->next;
	}
	list->outOfBounds = 1;
	list->curr = NULL;
	return NULL; // Item was not found in list, return NULL pointer
}

//*----end of list.h implementation----*//