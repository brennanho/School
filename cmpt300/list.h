//Node container with a type "data" item
typedef struct Node {
	void * item;
} Node;

//List with "size" elements of type Node
typedef struct LIST {
	Node* arr[10];
	void * currItem;
	int currIndex;
	int size;
} LIST;

//makes a new, empty list, and returns its reference on success. Returns a NULL pointer on failure.
LIST* ListCreate();

//returns the number of items in list
int ListCount(LIST* list);

//returns a pointer to the first item in list and makes the first item the current item.
void* ListFirst(LIST* list);

// returns a pointer to the last item in list and makes the last item the current one.
void* ListLast(LIST* list);

//advances list's current item by one, and returns a pointer to the new current item.
//if this operation advances the current item beyond the end of the list, a NULL pointer is returned.
void* ListNext(LIST* list);

//backs up list's current item by one, and returns a pointer to the new current item. 
//if this operation backs up the current item beyond the start of the list, a NULL pointer is returned.
void *ListPrev(LIST* list);

//returns a pointer to the current item in list.
void *ListCurr(LIST* list);

//adds the new item to list directly after the current item, and makes item the current item.
//if the current pointer is before the start of the list, the item is added at the start. 
//if the current pointer is beyond the end of the list, the item is added at the end.
//returns 0 on success, -1 on failure.
int ListAdd(LIST* list,void* item);

