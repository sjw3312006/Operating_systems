//Jeongwoon Suh : 301313489 : CMPT 300 : Assignment 2 (modified from Assignment 1)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

List hList[LIST_MAX_NUM_HEADS]; //List with maximum number of head (10)
Node hNode[LIST_MAX_NUM_NODES]; //List with maximum number of node (100)

int tempList = 0; 
int numNode = 0;

int mHead = LIST_MAX_NUM_HEADS;
int mNode = LIST_MAX_NUM_NODES;

//To test each of the functions in list.h, created the function that prints the list
int List_print(List *pList){

    if(pList == NULL){
        printf("List is Null\n");
        return LIST_FAIL;
    }
    if(pList -> head == NULL){
        printf("Head is NULL");
        return LIST_SUCCESS;
    }

    Node *curr = pList -> head;
    while((curr) != NULL){
        printf("%d\t", curr -> data);
        curr = curr -> next;
    }
    putchar('\n');
    return LIST_SUCCESS;
}

//Creating temporary node for future use
Node *Temp_node(void *pItem){ 

    Node *node;
    if(pItem == NULL){
        return NULL;
    }
    node = &hNode[numNode]; //using allocated list of nodes
    node -> data = pItem;
    node -> next = NULL;
    node -> previous = NULL;
    numNode++;
    return node;
};

List* List_create(){

    List *pList;

    if(pList == NULL){ //return null on failure
        return NULL;
    }

    pList = &hList[tempList]; //using allocated list
    pList -> count = 0; //indicating empty list
    pList -> current = NULL;
    pList -> head = NULL;
    pList -> tail = NULL;
    tempList++;
    return pList; //returns its reference on success
}

int List_count(List* pList){

    if(pList == NULL){
        return LIST_FAIL;
    }
    return pList -> count; //returns the number of items in pList
}

void* List_first(List* pList){

    if(pList == NULL || pList -> head == NULL){ //if list is empty
        pList -> current = NULL; //sets current item to NULL
        return NULL;
    }
    pList -> current = pList -> head; //first item
    return pList -> current -> data;
}

void* List_last(List* pList){

    if(pList == NULL || pList -> tail == NULL){ //if list is empty
        pList -> current = NULL; //sets current item to NULL
        return NULL;
    }
    pList -> current = pList -> tail; //last item
    return pList -> current -> data;
}

void* List_next(List* pList){
    
    Node * temp = pList -> current;
     if(pList == NULL || temp -> next == NULL || pList -> count == 0){ //if advances the current item beyond the end of the pList
        pList -> current = temp ->next; //current item is set to be beyond end of pList 
        return NULL;
    }
    temp = temp -> next; //advances current item by one
    pList -> current = temp; 
    return temp -> data;
}

void* List_prev(List* pList){

    Node * temp = pList -> current;
    if(pList == NULL || temp -> previous == NULL || pList -> count == 0){ //ㅑf backs up the current item beyond the start of the pList
        pList -> current = temp -> previous; //current item is set to be before the start of pList
        return NULL;
    }
    temp = temp -> previous; //backs up pList's current item by one
    pList -> current = temp;
    return temp -> data;
}

void* List_curr(List* pList){

    if(pList == NULL || pList -> count == 0){
        return NULL;
    }
    return pList -> current -> data; //returns a pointer to the current item in pList
}

int List_add(List* pList, void* pItem){

    Node *Add_node = Temp_node(pItem);

    if(pList == NULL || pItem == NULL || Add_node ==  NULL){
        return LIST_FAIL;
    }

    //If the current pointer is before the start of the pList
    if(pList -> head == NULL){
        pList -> head = Add_node;
        pList -> tail = Add_node;
        pList -> current = Add_node;
        pList -> count++;
        return LIST_SUCCESS; //the item is added at the start
    }

    //if the current pointer is beyond the end of the pList
    if(pList -> current -> next == NULL || pList -> current == pList -> tail){
        pList -> tail = Add_node; //the item is added at the end
    }
    else{
        pList -> current -> next -> previous = Add_node; //adding new item directly after the current item
    }

    //general case 
    Add_node -> next = pList -> current -> next;
    Add_node -> previous = pList -> current;
    pList -> current -> next = Add_node; //adding the new item directly after the current item
    pList -> current = Add_node; //making item the current item
    pList -> count++;

    return LIST_SUCCESS;
}

int List_insert(List* pList, void* pItem){
    Node *Insert_node = Temp_node(pItem);

    if(pList == NULL || pItem == NULL){
        return LIST_FAIL;
    }
        
    //if the current pointer is before the start of the pList
    if(pList -> head == NULL){
        pList -> head = Insert_node;
        pList -> tail = Insert_node;
        pList -> current = Insert_node; //the item is added at the start
        pList -> count++;
        return LIST_SUCCESS;
    }

    //if the item is the first one, insert item to the first
    if(pList -> current -> previous == NULL){
        pList -> head = Insert_node;
    }
    else{
        pList -> current -> previous -> next = Insert_node; //adding item directly before the current item
    }

    //general case
    Insert_node -> next = pList -> current;
    Insert_node -> previous = pList -> current -> previous;
    pList -> current -> previous = Insert_node; //adding item directly before the current item
    pList -> current = Insert_node; //making new item the current item
    pList -> count++;
    return LIST_FAIL;
}

int List_append(List* pList, void* pItem){

    if(pList == NULL || pItem == NULL){
        return LIST_FAIL;
    }
    pList -> current = pList -> tail; //making the last item to current item
    return List_add(pList, pItem); //adding item directly after the current item = adding item to the end of the list
}

int List_prepend(List* pList, void* pItem){

    if(pList == NULL || pItem == NULL){
        return LIST_FAIL;
    }
    pList -> current = pList -> head; //making the first item to current item
    return List_insert(pList, pItem); //adding item directly before the current item = adding item to the front of list 
}

void* List_remove(List* pList){ 

	if(pList == NULL || pList->current == NULL){
		return NULL;
	}

    Node * item_remove = pList -> current;
	int tmp_head = (pList -> current -> previous == NULL);
	int tmp_tail = (pList -> current -> next == NULL); 

	if(!tmp_head){ //If previous of pList is not NUll, then 
		pList -> current -> previous -> next = pList -> current -> next;
	}
    
	if(!tmp_tail){
		pList -> current -> next -> previous = pList -> current -> previous;
	}
   
	if(tmp_head){ //if previous of pList is NULL, then current points to next
		pList -> current = pList -> current -> next;
	}
	else if(tmp_tail){ //if pList of next is NULL, then points to previous
		pList -> current = pList -> current -> previous;
	}
	else{
		pList -> current = pList -> current -> next;
	}

	if(tmp_tail){
		pList -> tail = pList -> current;
	}
	if(tmp_head){
		pList -> head = pList -> current;
	}
	item_remove -> next = NULL;
	item_remove -> previous = NULL;
    pList -> count--;
	return item_remove;//->data;
}

void List_concat(List* pList1, List* pList2){

    if(pList1 == NULL || pList2 == NULL){ //both list don't exist
        return;
    }
    if(pList2 -> head == NULL){ //if plist2 doesn't exist
        return;
    }
    //appending pList1 to the end of pList1
    if(pList1 -> head == NULL){ //if plist 1 doesn't exist
        pList1 -> head = pList2 -> head;
        pList1 -> tail = pList2 -> tail;
        pList1 -> current = pList2 -> current; //the current pointer is set to the current pointer of pList1
        pList1 -> count = pList2 -> count;
        return;
    }

    pList1 -> tail -> next = pList2 -> head;
    pList2 -> head -> previous = pList1 -> tail;
    pList1 -> tail = pList2 -> tail;
    tempList--; //plist2 no longer exists
}

void List_free(List* pList, FREE_FN pItemFreeFn){

    Node * temp;
    if(pList == NULL || pList -> head == NULL){
        return;
    }
    List_first(pList);
    while(List_curr(pList) != NULL){
        temp = pList -> current;
        List_next(pList);
        pItemFreeFn = temp; //freeing the item
        List_remove(pList); //deleting the list
    }
    tempList++;
    mNode--; //all its node no longer exists after the operation

    pList -> tail = NULL;
    pList -> current = NULL;
}

void* List_trim(List* pList){
    if(pList == NULL){ //return NULL if list is initially empty
        return NULL;
    }
    List_last(pList); //return last item 
    return List_remove(pList); //take the last item out of list
}

void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){

    int search_item = pComparator(List_first(pList), pComparisonArg);
    if(pList == NULL || pComparator == NULL|| pComparisonArg == NULL || List_count(pList) == 0){
        return NULL;
    }
    if(pList -> current = pList -> head -> previous){ //the current pointer is before the start of the pList
        pList -> current = pList -> head;
        search_item = pComparator(List_curr(pList), pComparisonArg); //searching from the first node
    }
    while(!search_item && List_next(pList)){
        search_item = pComparator(List_curr(pList), pComparisonArg);
    }
    if(search_item){
        return List_curr(pList); //match is found, item returned
    }
    return NULL;
}