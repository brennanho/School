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

	void* item;
	int k = 62;
	item = &k;
	int firstAddTest = ListAdd(myList,item);

	printf("List count = %d\n",ListCount(myList));
	
	for (int i = 1; i < 5; i++) {
		item = &i;
		int addResult = ListAdd(myList,item);
	}

	Node* firstItem = ListFirst(myList);
	Node* lastItem = ListLast(myList);
	Node* nextCurr = ListNext(myList);

	PrintList(myList);
	printf("List first = %d\n",*(int*)(firstItem->item));
	printf("List last = %d\n",*(int*)(firstItem->item));
	printf("List next curr = %d\n",*(int*)(nextCurr->item));

	// printResources(heads,10,resourceHeads);
	// printResources(nodes,10,resourceNodes);

}