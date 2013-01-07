#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <suffixtree.h>
#include <suffixtree_config.h>
#include <suffixtree_statistic.h>
#include <suffixtree_fcs.h>

/*
 * Frequency Common Substring
 */

static fcs_item_s *fcs_create(suffix_tree_s *suffixtree)
{
	fcs_item_s *item;
	suffixtree_encode_e encode;
	item = (fcs_item_s*)malloc(sizeof(fcs_item_s));
	item->string = s16_stacksize_new(12, 6);
	item->leaf_total = 0;
	item->string_total = 0;
	encode = _suffixtree_get_encode(suffixtree);
	if (encode == suffixtree_encode_unicode)
		s16_set_encode(item->string, ENCODE_UNICODE);
	else if (encode == suffixtree_encode_gb2312)
		s16_set_encode(item->string, ENCODE_GB2312);
	return item;
}

static void fcs_free(fcs_item_s *item)
{
	s16_free(item->string);
	free(item);
}

static void fcs_clean(fcs_item_s *item)
{
	s16_free(item->string);
	free(item);
}

static int fcs_find(suffix_tree_s *suffixtree, suffix_node_s *node, size_t string_total, double threshold, sarray_s *array)
{
	suffix_node_s *edge = NULL;
	suffix_node_s *head_node;
	fcs_item_s *item;
	int stotal = _statistic_stringtotal(node);
	double score = (double)stotal / string_total;
	int find = 0;
	if (score >= threshold) {
		suffixnode_each_edge(edge, node) {
			find = fcs_find(suffixtree, edge, string_total, threshold, array);
		}
		suffixnode_each_end;
		/* find common substring */
		if (find == 1)
			return 1;
		/* no find common substring , current is common substring*/
		head_node = _suffixnode_head_node(suffixtree, node);
		item = fcs_create(suffixtree);
		item->state = FCS_STATE_SUBSTRING;
		textmatrix_getstr(suffixtree, item->string, head_node->sidx, node->eidx);
		item->sidx = head_node->sidx;
		item->eidx = node->eidx;
		item->leaf_total = _statistic_leaftotal(node);
		item->string_total = stotal;
		item->score = score;
		sarray_add_safe(array, (size_t)item);
		return 1;
	} else {
		return 0;
	}
}

static bool_t fcs_rule(sarray_s *array, fcs_item_s *fcs)
{
	fcs_item_s *item;
	sarray_each_type_m(item, fcs_item_s*, array) {
		/* skip self */
		if (item == fcs)
			continue;
		/* rule 1, range include */
		if (range_r1_include_r2_m(item->sidx, item->eidx, fcs->sidx, fcs->eidx)) {
			/* rule 2, score alike */
			if (item->score == fcs->score)
				return false_t;
		}
	}
	sarray_each_type_end_m;
	return true_t;
}

static void filter_substring_rule1_include(sarray_s *array)
{
	fcs_item_s *item;
	sarray_each_type_m(item, fcs_item_s*, array) {
		if (fcs_rule(array, item))
			item->state = FCS_STATE_FILTER_INCLUDE_SUBSTRING;
	}
	sarray_each_type_end_m;
}

static void filter_substring_rule2_no_one_word(sarray_s *array)
{
	fcs_item_s *item;
	sarray_each_type_m(item, fcs_item_s*, array) {
		if (s16_len(item->string) == 1)
			item->state = FCS_STATE_SUBSTRING;
	}
	sarray_each_type_end_m;
}

static void filter_substring(sarray_s *array)
{
	filter_substring_rule1_include(array);
	filter_substring_rule2_no_one_word(array);
}

void _suffixtree_fcs(suffix_tree_s *suffixtree, double threshold, sarray_s *array)
{
	suffix_node_s *root = suffixtree->root;
	suffix_node_s *edge;
	_suffixtree_statistic(suffixtree);
	suffixnode_each_edge(edge, root) {
		fcs_find(suffixtree, edge, suffixtree->string_id, threshold, array);
	}
	suffixnode_each_end;
	filter_substring(array);
}

void _suffixtree_fcs_string_array(sarray_s *fcs, sarray_s *strarray)
{
	fcs_item_s *item;
	sarray_each_type_m(item, fcs_item_s*, fcs) {
		sarray_add_safe(strarray, (size_t)item->string);
	}
	sarray_each_end_m;
}

void _suffixtree_fcs_string_array_state(sarray_s *fcs, sarray_s *strarray, state_t state)
{
	fcs_item_s *item;
	sarray_each_type_m(item, fcs_item_s*, fcs) {
		if (_fcs_item_state_m(item) < state)
			continue;
		sarray_add_safe(strarray, (size_t)item->string);
	}
	sarray_each_end_m;
}

void _suffixtree_fcs_clean(sarray_s *fcs)
{
	fcs_item_s *item;
	sarray_each_type_m(item, fcs_item_s*, fcs) {
		fcs_free(item);
	}
	sarray_each_end_m;
}
