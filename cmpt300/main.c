#include<stdio.h>
#include <stdlib.h>
#include "list.h"

//Helper function used to add an item to an empty list
void AddEmpty(LIST* list, void* item);

//Helper function used for printing contents of a list
void PrintList(LIST* list);

//Used for ListSearch
int comparator(void* item, void* comparisonArg);

//main test driver for list
int main(void) {

	LIST* myList = ListCreate();
	LIST* myList2 = ListCreate();

	for (int i = 0; i < 5; i++) {
		int* item = malloc(sizeof* item);
		*item = i;
		int preResult = ListPrepend(myList,item);
		int appResult = ListAppend(myList2,item);
	}

	PrintList(myList);
	ListFirst(myList);
	PrintList(myList);
	ListLast(myList);
	PrintList(myList);

	for (int i = 0; i < ListCount(myList); i++) {
		ListPrev(myList);
		PrintList(myList);
	}

	for (int i = 0; i < ListCount(myList)/2; i++) {
		ListNext(myList);
		PrintList(myList);
	}

	int* item = malloc(sizeof *item);
	int i = 42;
	*item = i;

	int* item2 = malloc(sizeof *item);
	int k = 77;
	*item2 = k;

	ListInsert(myList,item);
	PrintList(myList);
	ListAdd(myList,item2);
	PrintList(myList);
	ListRemove(myList);
	PrintList(myList);

	for (int i = 0; i < 10; i++) {
		ListRemove(myList);
	}

	PrintList(myList);

	// ListConcat(myList,myList2);

	// PrintList(myList);
	// PrintList(myList2);

	// // ListSearch(myList,comparator,item3);
	// // PrintList(myList);

	// printf("list index = %d\n",myList->index);
	// printf("list2 index = %d\n",myList2->index);
	return 0;
}