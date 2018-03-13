#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
	struct Node* next;
	struct Node* prev;
} Node;

int main (void) {

	Node myNode;
	Node myNext;
	Node myPrev;
	
	printf("%d\n",myNode);
	printf("%d\n",myNext);
	printf("%d\n",myPrev);

	myNode.next = myNext;
	myNode.prev = myPrev;

	printf("%d\n",myNode.next.prev);
	printf("%d\n",myNode.prev.next);

	return 0;
}