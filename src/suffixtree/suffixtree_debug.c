#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string16.h>
#include <sarray.h>
#include <suffixtree.h>
#include <suffixtree_config.h>
#include <suffixtree_debug.h>

void _suffixtree_out_textmatrix(suffix_tree_s *suffixtree)
{
	textmatrix_s *matrix;
	size_t r, c;
	_16b __16b;
	if (suffixtree == NULL || suffixtree->matrix == NULL)
		return;
	matrix = suffixtree->matrix;
	printf("Text Matrix:\n");
	printf(" Column Size:%d\n", matrix->column_size);
	printf(" Row Size:%d\n", matrix->row_size);
	printf(" Column Index:%d\n", matrix->cidx);
	printf(" Row Index:%d\n", matrix->ridx);
	for (c = 0; c < matrix->cidx+1; ++c) {
		for (r = 0; r < matrix->row_size; ++r) {
			__16b = matrix->matrix[c][r];
			if (c == matrix->cidx && r > matrix->ridx)
				break;
			printf(" {[%d][%d]%d", c, r, __16b);
			printf("(");
			if (suffixtree->encode == suffixtree_encode_unicode)
				s16b_out(__16b, ENCODE_UNICODE);
			else if (suffixtree->encode == suffixtree_encode_gb2312)
				s16b_out(__16b, ENCODE_GB2312);
			printf(")}");
		}
		printf("\n");
	}
}

static void out_usernode(suffix_node_s *node)
{
	suffixnode_usernode_s *usernode;
	list_each_data_m(usernode, suffixnode_usernode_s*, &(node->user_list)) {
		if (usernode->op != NULL &&
		    usernode->op->out != NULL) {
			usernode->op->out(usernode->data);
		}
	}
	list_each_data_end_m;
}

static void out_suffixnode(suffix_node_s *node, int level)
{
	int i;
	for (i = 0; i < level; ++i) {
		printf("\t");
	}
	if (node->edgesize != 0) {	
		printf("[Node] [StartIndex:%d] [EndIndex:%d] [EdgeSize:%d]", 
		       node->sidx,
		       node->eidx,
		       node->edgesize);
		out_usernode(node);
		printf("\n");
		for (i = 0; i < node->edgesize; ++i) {
			out_suffixnode(node->edges[i], level+1);
		}
	} else {
		printf("[Leaf] [StartIndex:%d] [EndIndex:%d] [StringID:%d] [SubStringId:%d]", 
		       node->sidx,
		       node->eidx,
		       node->string_id,
		       node->substring_id);
		out_usernode(node);
		printf("\n");
	}	
}

void _suffixtree_out_suffixtree(suffix_tree_s *suffixtree)
{
	out_suffixnode(suffixtree->root, 0);
}

static void out_suffixnode_text(suffix_tree_s *suffixtree, suffix_node_s *node, string16 *str)
{
	int i;
	string16 curstr, outstr;
	suffixtree_encode_e encode;

	s16_resource_init(&curstr);
	s16_resource_init(&outstr);
	
	/* get current node string */
	textmatrix_getstr(suffixtree, &curstr, node->sidx, node->eidx);

	/* combination string */
	s16_appends16(&outstr, str);
	s16_appends16(&outstr, &curstr);
	
	/* is leaf node */
	if (node->edgesize == 0) {
		encode = _suffixtree_get_encode(suffixtree);
		if (encode == suffixtree_encode_unicode)
			s16_set_encode(&outstr, ENCODE_UNICODE);
		else if (encode == suffixtree_encode_gb2312)
			s16_set_encode(&outstr, ENCODE_GB2312);
		printf("[StringID:%d] [SubStringID:%d]\t[", node->string_id, node->substring_id);
		s16_out(&outstr);
		printf("]\n");
	} else {
		for (i = 0; i < node->edgesize; ++i) {
			out_suffixnode_text(suffixtree, node->edges[i], &outstr);
		}
	}
	
	s16_resource_clear(&curstr);
	s16_resource_clear(&outstr);
}

void _suffixtree_out_suffixtree_string(suffix_tree_s *suffixtree)
{
	string16 s16;
	s16_resource_init(&s16);
	out_suffixnode_text(suffixtree, suffixtree->root, &s16);
	s16_resource_clear(&s16);
}
