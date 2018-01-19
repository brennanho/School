#include<stdio.h>
#include <stdlib.h>
#include "list.c"

//Pulling from declaration in list.c
extern Head* heads[10];
extern Node* nodes[10];

//For printing out the static nodes array and heads array
// void printResources(Node* arr[], int len, char type[]) {
// 	printf(" %c = [",*type);
// 	for (int i = 0; i < len-1; i++) {
// 		printf("%d,",arr[i]->inUse);
// 	}
// 		printf("%d]\n",arr[len-1]->inUse);
// }

int main(void) {

	//Initialize pool of heads
	for (int i = 0; i < 10; i++) {
		Head* newHead = malloc(sizeof* newHead);
		heads[i] = newHead;
	}		

	//Initialize pool of nodes
	for (int i = 0; i < 10; i++) {
		Node* newNode = malloc(sizeof* newNode);
		nodes[i] = newNode;
	}


	LIST* myList = ListCreate();
	LIST* myList2 = ListCreate();

	char* resourceHeads = "H";
	char* resourceNodes = "N";
	
	for (int i = 0; i < 10; i++) {
		int* item = malloc(sizeof* item);
		*item = i;
		int preResult = ListPrepend(myList,item);
		int appResult = ListAppend(myList,item);
		printf("preResult = %d, appResult = %d\n",preResult,appResult);
	}

	Node* firstItem = ListFirst(myList);
	Node* lastItem = ListLast(myList);
	Node* listNext = ListNext(myList);
	Node* listPrev = ListPrev(myList);
	//listNext = ListNext(myList);
	int size = ListCount(myList);

	PrintList(myList);
	printf("\nList first = %d\n",*(int*)(firstItem->item));
	printf("List last = %d\n",*(int*)(lastItem->item));
	printf("List curr = %d\n",*(int*)myList->curr->item);
	//printf("List next = %d\n",*(int*)(listNext->item));
	//printf("List curr = %d\n",myList->curr->next);

	printf("List size = %d\n",size);

	// printResources(heads,10,resourceHeads);
	// printResources(nodes,10,resourceNodes);

}