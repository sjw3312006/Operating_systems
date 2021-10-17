//Jeongwoon Suh : 301313489 : CMPT 300 : Assignment 1

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

int main(){

    printf("\nJeongwoon Suh : 301313489 : CMPT 300 : Assignment 1\n");
    printf("Test Function\n");

    printf("-------------------------------------------------------------\n");
    List * L = List_create(); 

    printf("\nCreating List\n");
	List_prepend(L, (void *) 1);
    List_prepend(L, (void *) 2); 
    List_prepend(L, (void *) 3);
    List_prepend(L, (void *) 4);
    List_insert(L, (void*) 5);
    List_append(L, (void*) 6);
    List_add(L, (void*) 7);

    List_print(L);

	printf("Number of items in the list: %d\n", List_count(L));
    printf("List trim is : %d\n", List_trim(L));
    printf("First item in the list: %d\n", List_first(L));
    printf("Current item in the list is: %d\n", List_curr(L));
    printf("Next item is: %d\n", List_next(L));
    printf("Current item in the list is: %d\n", List_curr(L));
    printf("First item in the list: %d\n", List_first(L));
    printf("Last item in the list: %d\n", List_last(L));
    printf("Previous item is: %d\n", List_prev(L));
	printf("Next item is: %d\n", List_next(L));


    printf("-------------------------------------------------------------\n");
    List * L1 = List_create();

    printf("\nCreating List2\n");
    List_insert(L1, (void *) 10);
	List_prepend(L1, (void *) 11);
	List_insert(L1, (void *) 12);
	List_add(L1, (void *) 13);	
	List_insert(L1, (void *) 14);	
	List_append(L1, (void *) 15);
	List_prepend(L1, (void *) 16);
	List_add(L1, (void *) 17);
    // List_search(L1, (void *) 13, (void *) 15);

	// List_remove(L1);
	// List_trim(L1);
	// List_trim(L1);

	List_print(L1);

    printf("First item in the list: %d\n", List_first(L1));
    printf("Next item is: %d\n", List_next(L1));
    printf("Next item is: %d\n", List_next(L1));
    // printf("List trim is : %d\n", List_trim(L1));
    printf("List remove is: %d\n", List_remove(L1));
	printf("Number of items in the list: %d\n", List_count(L1));
	printf("Current item in the list is: %d\n", List_curr(L1));
	printf("First item in the list: %d\n", List_first(L1));
	printf("Last item in the list: %d\n", List_last(L1));
	printf("Previous item is: %d\n", List_prev(L1));
	printf("Next item is: %d\n", List_next(L1));


    printf("-------------------------------------------------------------\n");
    printf("\nConcatenating List and List2 \n");
    List_concat(L, L1);
    List_print(L);
}
