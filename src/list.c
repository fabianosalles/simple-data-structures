#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>


void free_node_data(CallbackFree free_callback, void *data) {
	if (free_callback != NULL)
		free_callback(data);
	else
		free(data);
}

List *list_create(int dataSize, CallbackFree free_callback, CallbackCompare compare_callback) {
	assert(dataSize > 0);	

	List *list = (List *)malloc(sizeof(List));
	list->count = 0;
	list->data_size = dataSize;
	list->head = NULL;
	list->tail = NULL;
	list->callback_free = free_callback;
	list->callback_compare = compare_callback;
	return list;
}

void list_destroy(List *list){
	assert(list != NULL);
	
	if (list->head != NULL) {
		Node *node = list->head;
		while (node != NULL){
			list->head = node->next;
			free_node_data(list->callback_free, node->data);
			free(node);
			list->count--;
			node = list->head;
		}
	}
	free(list);
}


void list_add(List *list, void *data) {
	assert(list != NULL);
	assert(data != NULL);
	
	Node *newNode = calloc(sizeof(Node), 1);
	newNode->data = malloc(list->data_size);
	memcpy(newNode->data, data, list->data_size);	
	if (list->head == NULL) {
		list->head = newNode;
		list->tail = newNode;
	}
	else {
		list->tail->next = newNode;
		list->tail = newNode;
	}
	list->count++;
}

void list_remove(List *list, int index) {
	assert(list != NULL);
	assert(index >= 0);
	assert(index <= list->count-1);
	
	int i = 0;
	Node *curr_node = list->head;
	while (i < index-1){
		curr_node = curr_node->next;
		i++;
	}

	Node *target_node = curr_node->next;
	if (target_node == NULL) {
		list->tail = curr_node;
	}
	else {
		curr_node->next = target_node->next;
		if (target_node->next == NULL) 
			list->tail = curr_node;

		free_node_data(list->callback_free, target_node->data);		
		free(target_node);
	}	
	list->count--;
}


/* makes a full list iteration since our list it is not sorted */
bool list_contains(List *list, void *data) {
	assert(list->callback_compare != NULL);
	
	Node *node = list->head;
	while (node->next != NULL)	{
		if (list->callback_compare(node->data, data) == 0)
			return true;		
		node = node->next;
	}
	return false;
}

Node* list_find(List *list, void *data) {
	assert(list->callback_compare != NULL);

	Node *node = list->head;
	while (node->next != NULL) {
		if (list->callback_compare(node->data, data) == 0)
			return node;
		node = node->next;
	}
	return NULL;
}

Node* list_get_node(List *list, int index) {
	assert(list != NULL);
	assert(index < list->count);
	
	Node *node = list->head;
	for (int i = 0; i < index, node!= NULL; i++) {
		if (index == i) 
			return node;		
		node = node->next;
	}
	return NULL;
}

void* list_get_data(List *list, int index) {
	return list_get_node(list, index)->data;
}

void list_iterate(List *list, CallbackIterate iterate_callback) {
	assert(list != NULL);
	assert(iterate_callback != NULL);

	int i = 0;
	Node *node = list->head;
	while (node!= NULL){
		if (!iterate_callback(i, node))
			return;		
		node = node->next;
		i++;
	}
}
