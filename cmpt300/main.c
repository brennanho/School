#include<stdio.h>
#include <stdlib.h>
#include "list.h"

//Helper function used for printing contents of a list
void PrintList(LIST* list);

//Used for ListSearch
int comparator(void* item, void* comparisonArg);

//Used for ListFree
void itemFree(void* itemToBeFreed);

//main test driver for list.c/list.h
int main(void) {

	printf("\n----------BEGIN TEST----------\n\n");

	// START TEST: ListCreate
	int myListCap = 10;
	LIST* myList = ListCreate();
	LIST* myList2 = ListCreate();
	LIST* myList3 = ListCreate(); //empty list, used for concat test
	// END TEST: ListCreate

	printf("initializing items to myList (prepend)...\n\n");

	// START TEST: ListPrepend (initialize list)
	for (int i = 0; i < myListCap; i++) {
		int* item = malloc(sizeof *item);
		*item = i;
		ListPrepend(myList,item);
		PrintList(myList);
	}
	// END TEST: ListPrepend (initialize list)

	printf("\nmyList initialization complete\n\n");

	// START TEST: ListLast, ListFirst
	printf("current item == first item: ");
	ListFirst(myList);
	PrintList(myList);
	printf("current item == last item: ");
	ListLast(myList);
	PrintList(myList);
	// END TEST: ListLast, ListFirst

	printf("\nListPrev test up to last item...\n\n");

	// START TEST: ListNext, ListPrev
	//Move current down to the first item itteratively
	for (int i = 0; i < ListCount(myList); i++) {
		PrintList(myList);
		ListPrev(myList);
	}

	ListNext(myList);
	printf("\nListNext test up to half way through list...\n\n");

	//Move current up to the middle of myList itteratively
	for (int i = 0; i < ListCount(myList)/2; i++) {
		PrintList(myList);
		ListNext(myList);
	}
	// END TEST: ListNext, ListPrev

	// START TEST: ListCurr
	PrintList(myList);
	void* curr = ListCurr(myList);
	printf("\nListCurr test => %d\n",*(int*)curr);
	// END TEST: ListCurr

	// START TEST: ListInsert, ListAdd, ListAppend, ListPrepend
	int* itemIns = malloc(sizeof *itemIns);
	int i = 42;
	*itemIns = i;
	int* itemAdd = malloc(sizeof *itemAdd);
	int j = 77;
	*itemAdd = j;
	int* itemPrep = malloc(sizeof *itemPrep);
	int k = 62;
	*itemPrep = k;
	int* itemApp = malloc(sizeof *itemApp);
	int l = 81;
	*itemApp = l;
	int* itemNotInList = malloc(sizeof *itemNotInList);
	int m = 99;
	*itemNotInList = m;

	printf("\nListInsert(%d) test: ",i);
	ListInsert(myList,itemIns);
	PrintList(myList);
	printf("ListAdd(%d) test: ",j);
	ListAdd(myList,itemAdd);
	PrintList(myList);
	printf("ListPrepend(%d) test: ",k);
	ListPrepend(myList,itemPrep);
	PrintList(myList);
	printf("ListAppend(%d) test: ",l);
	ListAppend(myList,itemApp);
	PrintList(myList);
	// END TEST: ListInsert, ListAdd, ListAppend, ListPrepend

	// START TEST: ListSearch
	ListFirst(myList); 	//Begin search at the start of the list
	printf("ListSearch(%d) test 1 (item is in myList): ",i);
	ListSearch(myList,comparator,itemIns);
	PrintList(myList);
	ListFirst(myList); 	//Begin search at the start of the list
	printf("ListSearch(%d) test 2 (item is not in myList): ",m);
	ListSearch(myList,comparator,itemNotInList);
	PrintList(myList);
	// END TEST: ListSearch
	
	// START TEST: ListRemove
	printf("ListRemove test 1 (current item is beyond the end of myList): ");
	int* itemToFree = ListRemove(myList);
	PrintList(myList);
	printf("ListRemove test 2 (current item is the first item (62) in myList): ");
	ListFirst(myList);
	itemToFree = ListRemove(myList);
	free(itemToFree);
	PrintList(myList);
	// END TEST: ListRemove

	// START TEST: ListTrim
	printf("\nCalling ListTrim until list is empty...\n\n");
	while (ListCount(myList) != 0) {
		int* itemToTrim = ListTrim(myList);
		PrintList(myList);
		free(itemToTrim);
	}
	// END TEST: ListTrim

	// START TEST: ListInsert (initialize list)
	printf("\nInserting items back into list...\n\n");
	for (int i = 0; i < myListCap; i++) {
		int* item = malloc(sizeof* item);
		*item = i;
		ListInsert(myList,item);
		PrintList(myList);
	}
	// END TEST: ListInsert (initialize list)

	// START TEST: ListFree, ListCreate
	PrintList(myList);
	printf("\nListFree test result => ");
	ListFree(myList,itemFree);
	PrintList(myList);
	myList = ListCreate();
	// END TEST: ListFree, ListCreate

	// START TEST: ListAppend, ListPrepend (initialize list)
	printf("\nInitializing myList (appending) and myList2 (prepending)...\n\n");
	for (int i = 0; i < myListCap; i++) {
		int* item = malloc(sizeof* item);
		*item = i;
		ListAppend(myList,item);
		ListPrepend(myList2,item);
		PrintList(myList);
		PrintList(myList2);
	}
	// END TEST: ListAppend, ListPrepend (initialize list)

	// START TEST: ListConcat, ListCount, ListFree
	printf("\n%d items in myList = ",ListCount(myList));
	PrintList(myList);
	printf("%d items in myList2 = ",ListCount(myList2));
	PrintList(myList2);
	printf("%d items in myList3 = ",ListCount(myList3));
	PrintList(myList3);

	printf("\nListConcat myList2 to myList: ");
	ListConcat(myList,myList2);
	PrintList(myList);

	printf("ListConcat mylist3 to mylist: ");
	ListConcat(myList,myList3);
	PrintList(myList);

	ListFree(myList,itemFree);

	printf("\nListFree => %d items in myList = ",ListCount(myList));
	PrintList(myList);
	printf("ListFree => %d items in myList2 = ",ListCount(myList2));
	PrintList(myList2);
	printf("ListFree => %d items in myList3 = ",ListCount(myList3));
	PrintList(myList3);

	printf("\n----------TEST COMPLETE----------\n");
	// END TEST: ListConcat, ListCount, ListFree

	return 0;
}