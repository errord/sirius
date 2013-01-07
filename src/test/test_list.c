#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <easycheck.h>
#include <list.h>

int main()
{
	list_s *list;
	list_node_s *node;
	char *info = "List";
	char *data = "abcd";
	char *v;
	char *check[7] = {
		"0", "1", "2", "2.5", "3", "abcd", "tail"
	};
	char buf[1024];
	int i = 0;
	echeck_start(info);
	list = list_new(NULL);
	list_insert_head(list, "1");
	list_insert_head(list, "0");
	node = list_insert_tail(list, "2");
	list_insert_tail(list, "3");
	list_insert(node, "2.5");
	node = list_insert_tail(list, data);
	node = list_insert(node, "tail");
	list_tail_m(list) = node;
	v = (char*)node_data_m(list_head_m(list));
	ECHECK("head", !strcmp(v,"0"));
	v = (char*)node_data_m(list_tail_m(list));
	ECHECK("tail", !strcmp(v,"tail"));
	list_each_m(node, list) {
		v = (char*)node_data_m(node);
		sprintf(buf, "check %s == node %s", check[i], v);
		ECHECK(buf, !strcmp(v, check[i]));
		i++;
	}
	list_each_end_m;
	echeck_end();
	return 0;
}
