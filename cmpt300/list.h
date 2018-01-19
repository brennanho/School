//Node container with a type "data" item
typedef struct Node {
	void * item;
	struct Node* next;
	struct Node* prev;
} Node;

typedef struct Head {
	struct Node* first;
} Head;

//List with "size" elements of type Node
typedef struct LIST {
	Node * curr;
	Head* head; // Pointer to the first item in the list
	Node* first;
	Node* last;
	int size;
	int outOfBounds; // current pointer: 0 -> in bounds, -1 -> before the start of the list, 1 -> after the end of the list
} LIST;

//makes a new, empty list, and returns its reference on success. Returns a NULL pointer on failure.
LIST* ListCreate();

//Helper function used to add an item to an empty list
void AddEmpty(LIST* list, void* item);

void UpdateLast(LIST* list);

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

//adds item to list directly before the currentitem, and makes the new item the current one. 
//If the current pointer is before the start of the list, the item is added at the start. 
//If the current pointer is beyond the end of the list, the item is added at the end. Returns 0 on success, -1 on failure.
int ListInsert(LIST* list,void* item);

// adds item to the end of list, and makes the new item the current one.
// Returns 0 on success, -1 on failure
int ListAppend(LIST* list,void* item);

// adds item to the front of list, and makes the new item the current one.
// Returns 0 on success, -1 on failure
int ListPrepend(LIST* list,void* item);

//Return current item and take it out of list. Make the next item the current one.
void *ListRemove(LIST* list); 

//adds list2 to the end of list1. The current pointer is set to the current pointer of list1.
//List2 no longer exists after the operation.
void ListConcat(LIST* list1,LIST* list2);

// delete list. itemFree is a pointer to a routine that frees an item. 
//It should be invoked (within ListFree) as: (*itemFree)(itemToBeFreed);
//void ListFree(LIST* list, itemFree);

//Return last item and take it out of list. Make the new last item the current one.
void *ListTrim(LIST* list);

//void *ListSearch(list, comparator, comparisonArg);