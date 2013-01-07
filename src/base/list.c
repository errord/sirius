#include <stdlib.h>
#include <stdio.h>
#include <list.h>

static list_node_s *_new_node(void *data)
{
	list_node_s *node;
	node = (list_node_s*)malloc(sizeof(list_node_s));
	node->data = data;
	node->next = NULL;
	return node;
}

static void default_node_resource_free(void *data)
{
	free(data);
}

list_s *list_new(list_node_resource_free_f resource_free)
{
	list_s *list;
	list = (list_s*)malloc(sizeof(list_s));
	list_resource_init(list, resource_free);
	return list;
}

void list_resource_init(list_s *list, list_node_resource_free_f resource_free)
{
	list->head = NULL;
	list->tail = NULL;
	list->resource_free = resource_free != NULL ? resource_free : default_node_resource_free;
}

void list_resource_clear(list_s *list)
{
	list_node_s *node, *temp;
	node = list->head;
	while (node) {
		list->resource_free(node->data);
		temp = node;
		node = node->next;
		free(temp);		
	}	
}

void list_free(list_s *list)
{
	list_resource_clear(list);
	free(list);
}

list_node_s *list_insert(list_node_s *node, void *data)
{
	list_node_s *next = _new_node(data);
	next->next = node->next;
	node->next = next;
	return next;
}

list_node_s *list_insert_head(list_s *list, void *data)
{
	list_node_s *node = _new_node(data);
	if (list->head == NULL) {
		list->head = list->tail = node;
	} else {
		node->next = list->head;
		list->head = node;
	}
	return node;
}

list_node_s *list_insert_tail(list_s *list, void *data)
{
	list_node_s *node = _new_node(data);
	if (list->tail == NULL) {
		list->head = list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
	return node;
}

