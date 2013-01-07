#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <suffixtree.h>
#include <suffixtree_statistic.h>
#include <suffixtree_fcs.h>
#include <suffixtree_debug.h>
#include <libsirius_suffixtree.h>

#define to_suffixtree(p) ((suffix_tree_s*)p)
#define to_handle(p) ((suffixtree)p)


suffixtree suffixtree_create()
{
	assert(sizeof(suffixtree) == sizeof(suffix_tree_s*));	
	return to_handle(_suffixtree_create());
}

void suffixtree_clean(suffixtree suffixtree)
{
	_suffixtree_clean(to_suffixtree(suffixtree));
}

void suffixtree_use_node_cache(suffixtree suffixtree, int use)
{
	_suffixtree_use_node_cache(to_suffixtree(suffixtree), use);
}

int suffixtree_is_use_node_cache(suffixtree suffixtree)
{
	return _suffixtree_is_use_node_cache(to_suffixtree(suffixtree));
}

void suffixtree_set_encode(suffixtree suffixtree, suffixtree_encode_e encode)
{
	_suffixtree_set_encode(to_suffixtree(suffixtree), encode);
}

suffixtree_encode_e suffixtree_get_encode(suffixtree suffixtree)
{
	return _suffixtree_get_encode(to_suffixtree(suffixtree));
}

void suffixtree_insert(suffixtree suffixtree, string16 *s)
{
	_suffixtree_insert(to_suffixtree(suffixtree), s);
}

void suffixtree_statistic(suffixtree suffixtree)
{
	_suffixtree_statistic(to_suffixtree(suffixtree));
}

/* FCS */

void suffixtree_fcs(suffixtree suffixtree, double threshold, sarray_s *array)
{
	_suffixtree_fcs(to_suffixtree(suffixtree), threshold, array);
}

void suffixtree_fcs_clean(sarray_s *fcs)
{
	_suffixtree_fcs_clean(fcs);
}

void suffixtree_fcs_string_array(sarray_s *fcs, sarray_s *fcs_str)
{
	_suffixtree_fcs_string_array(fcs, fcs_str);
}

void suffixtree_fcs_string_array_state(sarray_s *fcs, sarray_s *fcs_str, state_t state)
{
	_suffixtree_fcs_string_array_state(fcs, fcs_str, state);
}

state_t suffixtree_fcs_item_state(fcs_item item)
{
	return _fcs_item_state_m((fcs_item_s*)item);
}

string16 *suffixtree_fcs_item_string(fcs_item item)
{
	return _fcs_item_string_m((fcs_item_s*)item);
}

size_t suffixtree_fcs_item_startidx(fcs_item item)
{
	return _fcs_item_sidx_m((fcs_item_s*)item);
}
size_t suffixtree_fcs_item_endidx(fcs_item item)
{
	return _fcs_item_eidx_m((fcs_item_s*)item);
}

int suffixtree_fcs_item_leaf_total(fcs_item item)
{
	return _fcs_item_leaftotal_m((fcs_item_s*)item);
}

int suffixtree_fcs_item_string_total(fcs_item item)
{
	return _fcs_item_stringtotal_m((fcs_item_s*)item);
}

double suffixtree_fcs_item_score(fcs_item item)
{
	return _fcs_item_score_m((fcs_item_s*)item);
}

/* Debug */

void suffixtree_out_textmatrix(suffixtree suffixtree)
{
	_suffixtree_out_textmatrix(to_suffixtree(suffixtree));
}

void suffixtree_out_suffixtree(suffixtree suffixtree)
{
	_suffixtree_out_suffixtree(to_suffixtree(suffixtree));
}

void suffixtree_out_suffixtree_string(suffixtree suffixtree)
{
	_suffixtree_out_suffixtree_string(to_suffixtree(suffixtree));
}
