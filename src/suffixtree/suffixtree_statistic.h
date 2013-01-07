#ifndef _SUFFIXTREE_STATISTIC_H_
#define _SUFFIXTREE_STATISTIC_H_

#define STATISTIC_TAG "SuffixTree_Statistic"

typedef struct {
	int leaf_total; /* leaf total */
	int string_total; /* string total */
} node_statistic_item_s;

typedef struct {
	size_t suffixtree_change_number;
} tree_statistic_item_s;

void _suffixtree_statistic(suffix_tree_s *suffixtree);
int _statistic_leaftotal(suffix_node_s *node);
int _statistic_stringtotal(suffix_node_s *node);

#endif /* _SUFFIXTREE_STATISTIC_H_ */
