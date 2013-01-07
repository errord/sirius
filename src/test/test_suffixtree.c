#include <stdlib.h>
#include <stdio.h>
#include <libsirius_suffixtree.h>

void out_fcs_str(sarray_s *array)
{
	string16 *s16;
	sarray_each_type_m(s16, string16*, array) {
		s16_out(s16);
		printf("\n");
	}
	sarray_each_end_m;
}


int main()
{
	suffixtree st;
	string16 *s16;
	sarray_s fcs_array;
	sarray_s fcs_str_array;
	sarray_resource_init(&fcs_array);
	sarray_resource_init(&fcs_str_array);
	s16 = s16_new(100);
	st = suffixtree_create();
	suffixtree_use_node_cache(st, 1);
	suffixtree_set_encode(st, suffixtree_encode_gb2312);
	s16_setcstr(s16, "中国人民银行");
	suffixtree_insert(st, s16);
	s16_setcstr(s16, "中国交通银行");
	suffixtree_insert(st, s16);
	s16_setcstr(s16, "中国建设银行");
	suffixtree_insert(st, s16);
	s16_setcstr(s16, "中国农业银行");
	suffixtree_insert(st, s16);
	suffixtree_statistic(st);
	suffixtree_out_textmatrix(st);
	suffixtree_out_suffixtree_string(st);
	suffixtree_fcs(st, 0.5, &fcs_array);
	suffixtree_out_suffixtree(st);
	suffixtree_fcs_string_array(&fcs_array, &fcs_str_array);
	out_fcs_str(&fcs_str_array);
	suffixtree_fcs_clean(&fcs_array);
	suffixtree_clean(st);
	s16_free(s16);
	sarray_resource_clear(&fcs_array);
	sarray_resource_clear(&fcs_str_array);
	return 0;
}
