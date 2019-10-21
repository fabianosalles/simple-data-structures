#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#define TST_ALLOCATIONS (100*1000)

int compare_chars(void *a, void *b){
	return *(char *)a - *(char *)b; 
}

int compare_floats(void *a, void *b) {
	return (int) (*(float *)a - *(float *)b);
}

int compare_ints(void *a, void *b) {
	return *(int *)a - *(int *)b;
}


List *make_list_of_ints(int count) {
	List *list = list_create(sizeof(int), NULL, compare_ints);
	for (int i = 0; i < count; i++) {
		list_add(list, &i);
	}
	return list;
}

void test_allocation() {
	printf("Testing list allocation with %d allocations... ", TST_ALLOCATIONS);
	List* list = make_list_of_ints(TST_ALLOCATIONS);
	list_destroy(list);
	printf("OK\n");
}

void test_with_chars(){
	printf("Testing list as a collection of chars... ");
	
	char c;
	List* list = list_create(sizeof(char), NULL, compare_chars);
	
	for(char i = 'A'; i <= 'F'; i++){
		list_add(list, &i);
	}
	
	assert(list->count == 6);
	
	c = 'C';
	assert(list_contains(list, &c));
	c = 'a';
	assert(!list_contains(list, &c));
	c = 'G';
	assert(!list_contains(list, &c));
		
	Node *node = list_get_node(list, 3);
	assert(*(char *)node->data == 'D');

	list_destroy(list);
	printf("OK\n");
}



void test_with_floats() {
	printf("Testing list as a collection of chars... ");

	float f;
	List* list = list_create(sizeof(float), NULL, compare_floats);

	for (int i=0; i< TST_ALLOCATIONS; i++){
		f = (float)i;
		list_add(list, &f);
	}

	f = 100.0;
	assert(list_contains(list, &f));

	list_destroy(list);
}

void test_remove() {
	printf("Testing list remove... ");
	List *list = make_list_of_ints(100);
	int x =80;
	assert(list_contains(list, &x));
	
	list_remove(list, &x);
	assert(!list_contains(list, &x));

	list_destroy(list);
	printf("OK\n");
}

int main(int argc, char *argv[]) {
	
	test_allocation();
	test_remove();
	test_with_chars();
	test_with_floats();
	return 0;
}
