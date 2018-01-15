#include<stdio.h>
#include <stdlib.h>
#include "list.c"

void printList(LIST* list) {
	printf("[ ");
	for (int i = 0; i < list->size; i++){
		printf("%d,",*(int*)list->arr[i]->item);
	}
	printf("]\n");
}

int main(void) {

	int data = 10;
	LIST* myList = ListCreate();
	printf("myList size = %d\n",ListCount(myList));

	void* first = ListFirst(myList);
	void* last = ListLast(myList);
	void* nextItem = ListNext(myList);
	void* prevItem = ListPrev(myList);
	void* currPtr = ListCurr(myList);

	printf("first item = %d\n", (void*)first);
	printf("last item = %d\n", (void*)last);
	printf("next item = %d\n", (void*)nextItem);
	printf("prev item = %d\n", (void*)prevItem);
	printf("current ptr = %d\n", (void*)currPtr);

	void* item;
	int a = 5;
	item = &a;
	
	int resultAdd = ListAdd(myList,item);
	resultAdd = ListAdd(myList,item);
	resultAdd = ListAdd(myList,item);
	resultAdd = ListAdd(myList,item);

	printList(myList);

}