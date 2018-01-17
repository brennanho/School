#include<stdio.h>
#include <stdlib.h>
#include "list.c"

//Pulling from declaration in list.c
extern Node* heads[10];
extern Node* nodes[10];

//For printing out the static nodes array and heads array
void printResources(Node* arr[], int len, char type[]) {
	printf(" %c = [",*type);
	for (int i = 0; i < len-1; i++) {
		printf("%d,",arr[i]->inUse);
	}
		printf("%d]\n",arr[len-1]->inUse);
}

int main(void) {

	//Initialize pool of heads
	for (int i = 0; i < 10; i++) {
		Node* newHead = malloc(sizeof* newHead);
		newHead->inUse = 0;
		heads[i] = newHead;
	}		

	//Initialize pool of nodes
	for (int i = 0; i < 10; i++) {
		Node* newNode = malloc(sizeof* newNode);
		newNode->inUse = 0;
		nodes[i] = newNode;
	}


	LIST* myList = ListCreate();
	LIST* myList2 = ListCreate();

	char* resourceHeads = "H";
	char* resourceNodes = "N";

	void* item;
	int num = 25;
	item = &num;

	printf("List count = %d\n",ListCount(myList));
	int addResult = ListAdd(myList,item);

	printf("item added =  %d\n",*(int*)myList->last->item);
	//printf("List first = %d\n",*(int*)ListFirst(myList));
	//printf("List last = %d\n",*(int*)ListLast(myList));

	printResources(heads,10,resourceHeads);
	printResources(nodes,10,resourceNodes);

}