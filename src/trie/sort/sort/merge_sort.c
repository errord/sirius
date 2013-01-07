#include <stdlib.h>
#include <stdio.h>

struct list_node
{
	int data;
	struct list_node *next;
};

static struct list_node *create_node(struct list_node *h, int data)
{
	struct list_node *p;
	p = (struct list_node*)malloc(sizeof(struct list_node));
	p->data = data;
	p->next = NULL;
	if (h != NULL)
		h->next = p;
	return p;
}

static void free_list(struct list_node *head)
{
	struct list_node *p;
	while (head) {
		p = head;
		head = head->next;
		free(p);
	}
}


static struct list_node *make_list(int *data, size_t size)
{
	struct list_node *head = NULL;
	struct list_node *list = NULL;
	size_t i;
	for (i = 0; i < size; i++) {
		list = create_node(list, data[i]);
		if (head == NULL)
			head = list;
	}
	return head;
}

static void print_list(struct list_node *list)
{
	while (list) {
		printf("%d ", list->data);
		list = list->next;
	}
}

struct list_node *merge_sort_list(struct list_node *l1, struct list_node *l2)
{
	struct list_node *merge, *flow, *stop, *tmp;
	if (l1->data < l2->data) {
		merge = l1;
		flow = l1;
		stop = l2;
	} else {
		merge = l2;
		flow = l2;
		stop = l1;
	}
	while (1) {
		while (flow->next != NULL && flow->next->data <= stop->data)
			flow = flow->next;
		if (flow->next == NULL)
			break;
		tmp = flow->next;
		flow->next = stop;
		flow = stop;
		stop = tmp;
	}

	flow->next = stop;
	return merge;
}


int main()
{
	int d1[] = {2, 5, 16, 23, 25, 45, 46, 56, 89, 299, 456, 2356};
	int d2[] = {23, 43, 45, 47, 48, 53, 72, 98, 240, 256, 3473, 4898};
	struct list_node *l1;
	struct list_node *l2;
	struct list_node *merge = NULL;
	// l1
	l1 = make_list(d1, sizeof(d1)/sizeof(int));
	printf("l1: [");
	print_list(l1);
	printf("]\n");
	// l2
	l2 = make_list(d2, sizeof(d2)/sizeof(int));
	printf("l2: [");
	print_list(l2);
	printf("]\n");
	// merge sort
	merge = merge_sort_list(l1, l2);
	printf("merge: [");
	print_list(merge);
	printf("]\n");
	free_list(merge);
	return 0;
}
