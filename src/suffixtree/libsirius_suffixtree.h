#ifndef _LIB_SIRIUS_SUFFIXTREE_H_
#define _LIB_SIRIUS_SUFFIXTREE_H_

#include <libsirius_base.h>

typedef enum {
	suffixtree_encode_unicode,
	suffixtree_encode_gb2312
} suffixtree_encode_e;

typedef void* suffixtree;
typedef void* fcs_item;

#define FCS_STATE_SUBSTRING 0x01
#define FCS_STATE_FILTER 0x02
#define FCS_STATE_FILTER_INCLUDE_SUBSTRING 0x03


/* Suffix Tree Api */
suffixtree suffixtree_create();
void suffixtree_clean(suffixtree suffixtree);
void suffixtree_insert(suffixtree suffixtree, string16 *s);
void suffixtree_statistic(suffixtree suffixtree);

/* FCS(Frequency Common Substring) Api */
void suffixtree_fcs(suffixtree suffixtree, double threshold, sarray_s *array);
void suffixtree_fcs_clean(sarray_s *fcs);
void suffixtree_fcs_string_array(sarray_s *fcs, sarray_s *fcs_str);
void suffixtree_fcs_string_array_state(sarray_s *fcs, sarray_s *fcs_str, state_t state);
state_t suffixtree_fcs_item_state(fcs_item item);
string16 *suffixtree_fcs_item_string(fcs_item item);
size_t suffixtree_fcs_item_startidx(fcs_item item);
size_t suffixtree_fcs_item_endidx(fcs_item item);
int suffixtree_fcs_item_leaf_total(fcs_item item);
int suffixtree_fcs_item_string_total(fcs_item item);
double suffixtree_fcs_item_score(fcs_item item);

/* Config Api */
void suffixtree_use_node_cache(suffixtree suffixtree, int use);
int suffixtree_is_use_node_cache(suffixtree suffixtree);
void suffixtree_set_encode(suffixtree suffixtree, suffixtree_encode_e encode);
suffixtree_encode_e suffixtree_get_encode(suffixtree suffixtree);


/* Plug In */

/* Debug Api */
void suffixtree_out_textmatrix(suffixtree suffixtree);
void suffixtree_out_suffixtree(suffixtree suffixtree);
void suffixtree_out_suffixtree_string(suffixtree suffixtree);


#endif /* _LIB_SIRIUS_SUFFIXTREE_H_ */
