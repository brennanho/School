#include<stdio.h>
#include <stdlib.h>
#include "list.c"

//Pulling from declaration in list.c
extern Node* heads[10];
extern Node* nodes[100];

//For printing out the static nodes array and heads array
void printResources(Node* arr[], int len) {
	printf("[");
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
	for (int i = 0; i < 100; i++) {
		Node* newNode = malloc(sizeof* newNode);
		newNode->inUse = 0;
		nodes[i] = newNode;
	}

	LIST* myList = ListCreate();
	LIST* myList2 = ListCreate();

	printResources(heads,10);
	//printResources(nodes,100);

	printf("List count = %d\n",ListCount(myList));
	printf("List first = %d\n",*(int*)ListFirst(myList));
	printf("List last = %d\n",*(int*)ListLast(myList));

}