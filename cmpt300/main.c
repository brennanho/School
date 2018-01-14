#include<stdio.h>

typedef struct Node {
	void * data;
} Node;

int main(void) {

	int data = 10;
	Node* item;
	item->data = &data;
	printf("item data = %d",*(int*)item->data);
}