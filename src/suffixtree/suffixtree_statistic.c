#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string16.h>
#include <sarray.h>
#include <suffixtree.h>
#include <suffixtree_config.h>
#include <suffixtree_statistic.h>

typedef struct {
	sarray_s *string_id_array;
} statistic_info_s;


static void node_out(void *data)
{
	node_statistic_item_s *item = (node_statistic_item_s*)data;
	printf(" [LeafTotal:%d] [StringTotal:%d]", item->leaf_total, item->string_total);
}

#ifndef WIN32
static sn_usernode_op_s sn_op = {
	out:node_out,
	datadelete:free
};

static st_usernode_op_s st_op = {
	out:NULL,
	datadelete:free,
	configdelete:NULL
};
#else
static sn_usernode_op_s sn_op = {
	/*out*/			node_out,
	/*datadelete*/		free
};
static st_usernode_op_s st_op = {
	/*out*/			NULL,
	/*datadelete*/		free,
	/*configdelete*/	NULL
};
#endif /* WIN32 */

static void statistic_traversal(suffix_node_s *node, node_statistic_item_s *item, statistic_info_s *info)
{
	suffix_node_s *edge;
	node_statistic_item_s *cur_item;
	statistic_info_s cur_info;
	cur_item = (node_statistic_item_s*)malloc(sizeof(node_statistic_item_s));
	cur_item->leaf_total = 0;
	cur_item->string_total = 0;
	cur_info.string_id_array = sarray_stacksize_new(1000, 100);
	if (node->edgesize == 0) {
		/* leaf count leaf total */
		cur_item->leaf_total = 1;
		if (item != NULL)
			item->leaf_total += 1;
		/* leaf count string id */
		cur_item->string_total = 1;
		if (info != NULL) {
			sarray_add_uniquely_safe(info->string_id_array, node->string_id);
		}
	} else {
		suffixnode_each_edge(edge, node) {
			statistic_traversal(edge, cur_item, &cur_info);
		}
		suffixnode_each_end;
		/* node count leaf total */
		if (item != NULL)
			item->leaf_total += cur_item->leaf_total;

		/* node count string id */
		cur_item->string_total = (int)sarray_len_m(cur_info.string_id_array);
		if (info != NULL)
			sarray_merge_union(info->string_id_array, cur_info.string_id_array);
	}
	_suffixnode_set_usernode(node, STATISTIC_TAG, (void*)cur_item, &sn_op, SUFFIXNODE_USERNODE_DELETE_EXIST);
	sarray_free(cur_info.string_id_array);
}

static int _check_suffixtree_change(suffix_tree_s *suffixtree, tree_statistic_item_s *item)
{
	if (_suffixtree_change_number(suffixtree) == item->suffixtree_change_number)
		return 0;
	return 1;
}

void _suffixtree_statistic(suffix_tree_s *suffixtree)
{
	tree_statistic_item_s *tree_item;
	tree_item = (tree_statistic_item_s*)_suffixtree_get_userdata(suffixtree, STATISTIC_TAG);
	if (tree_item != NULL) {
		if (!_check_suffixtree_change(suffixtree, tree_item))
			return;
		tree_item->suffixtree_change_number = _suffixtree_change_number(suffixtree);	
	} else {
		tree_item = (tree_statistic_item_s*)malloc(sizeof(tree_statistic_item_s));
		tree_item->suffixtree_change_number = _suffixtree_change_number(suffixtree);
		_suffixtree_set_usernode(suffixtree, STATISTIC_TAG, (void*)tree_item, NULL, &st_op, 0);
	}

	statistic_traversal(suffixtree->root, NULL, NULL);
}

#define get_node_item_m(node) ((node_statistic_item_s*)_suffixnode_get_userdata(node, STATISTIC_TAG))

int _statistic_leaftotal(suffix_node_s *node)
{
	return get_node_item_m(node)->leaf_total;
}

int _statistic_stringtotal(suffix_node_s *node)
{
	return get_node_item_m(node)->string_total;
}

