#ifndef	_SIRIUS_LIST_H_
#define	_SIRIUS_LIST_H_

/*
 * simpleness linked List 
 */

typedef void (*list_node_resource_free_f)(void *data);

typedef struct _l_node {
	void *data;
	struct _l_node *next;
} list_node_s;

typedef struct {
	list_node_s *head;
	list_node_s *tail;
	list_node_resource_free_f resource_free;
} list_s;

list_s *list_new(list_node_resource_free_f resource_free);
void list_resource_init(list_s *list, list_node_resource_free_f resource_free);
void list_resource_clear(list_s *list);
void list_free(list_s *list);
list_node_s *list_insert(list_node_s *node, void *data);
list_node_s *list_insert_head(list_s *list, void *data);
list_node_s *list_insert_tail(list_s *list, void *data);

#define node_data_m(node) (node)->data

#define list_head_m(list) (list)->head
#define list_head_data_m(list) node_data_m(list_head_m(list))

#define list_tail_m(list) (list)->tail
#define list_tail_data_m(list) node_data_m(list_tail_m(list))

#define list_next_m(node) (node)->next
#define list_next_data_m(node) node_data_m(list_next_m(node))

#define list_each_m(node, list) for(node = list_head_m(list); node; node = list_next_m(node))
#define list_each_end_m

#define list_each_data_m(data, type, list)				\
	{								\
		list_node_s *dss_node;					\
		for (dss_node = list_head_m(list); dss_node; dss_node = list_next_m(dss_node)) \
		{							\
			data = (type)node_data_m(dss_node);
#define list_each_data_end_m }}

#endif /* _SIRIUS_LIST_H_ */
