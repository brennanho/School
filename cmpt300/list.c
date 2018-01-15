#include "list.h"

LIST* ListCreate() {
	LIST* newList = malloc(sizeof *newList);
	newList->size = 0; // Refers to number of elements in the list
	newList->currIndex = 0;
	return newList;
}

int ListCount(LIST* list) {
	return list->size;
}

void* ListFirst(LIST* list) {
	list->currItem = list->arr[0]->item;
	list->currIndex = 0;
	return list->arr[list->currIndex]->item; // Equivalent to "return list->arr"
}

void* ListLast(LIST* list) {
	list->currItem = list->arr[list->size]->item;
	list->currIndex = list->size;
	return list->arr[list->currIndex]->item;
}

void* ListNext(LIST* list) {
	list->currIndex++;
	if (list->currIndex <= list->size) { //Advance stays within end of list
		list->currItem = list->arr[list->currIndex]->item;
		return list->currItem;
	} else { //Advance goes beyond the end of the list
		return NULL;
	}
}

void *ListPrev(LIST* list) {
	list->currIndex--;
	if (list->currIndex >= 0) { //Backup stays within the start of the list
		list->currItem = list->arr[list->currIndex]->item;
		return list->currItem;
	} else { //Backup goes beyond the start of the list
		return NULL;
	}
}

//returns a pointer to the current item in list.
void *ListCurr(LIST* list) {
	return list->currItem;
}

int ListAdd(LIST* list,void* item) {
	Node* node = malloc(sizeof *node);
	node->item = item;

	if (list->currIndex > list->size) //add item at the end of list 
	{
		list->arr[++list->size] = node;

	} else if (list->currIndex < 0) { // add item at the start of the list
		for (int i = list->size; i >= 0; i--) {
			list->arr[i+1] = list->arr[i];
		}
		list->size++;
		list->arr[list->currIndex] = node;

	} else { // add item at position after item
		for (int i = list->size; i > list->currIndex; i--) {
			list->arr[i+1] = list->arr[i];
		}
		list->currIndex++;
		list->size++;
		list->currItem = node->item;
		list->arr[list->currIndex] = node;

	}
	//printf("%d  \n",*(int*)list->arr[1]->item);
	return 0;
}