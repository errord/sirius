#ifndef _SUFFIXTREE_FCS_H_
#define _SUFFIXTREE_FCS_H_

typedef struct {
	state_t state;
	string16 *string;
	size_t sidx;
	size_t eidx;
	int leaf_total; /* leaf total */
	int string_total; /* string total */
	double score;
} fcs_item_s;

void _suffixtree_fcs(suffix_tree_s *suffixtree, double threshold, sarray_s *array);
void _suffixtree_fcs_clean(sarray_s *fcs);
void _suffixtree_fcs_string_array(sarray_s *fcs, sarray_s *strarray);
void _suffixtree_fcs_string_array_state(sarray_s *fcs, sarray_s *strarray, state_t state);

#define _fcs_item_state_m(item) ((item)->state)
#define _fcs_item_string_m(item) ((item)->string)
#define _fcs_item_sidx_m(item) ((item)->sidx)
#define _fcs_item_eidx_m(item) ((item)->eidx)
#define _fcs_item_leaftotal_m(item) ((item)->leaf_total)
#define _fcs_item_stringtotal_m(item) ((item)->string_total)
#define _fcs_item_score_m(item) ((item)->score)

#endif /* _SUFFIXTREE_FCS_H_ */
