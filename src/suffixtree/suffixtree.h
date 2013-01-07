#ifndef _SUFFIX_TREE_H_
#define _SUFFIX_TREE_H_

#include <suffixtree_config.h>
#include <libsirius_suffixtree.h>

typedef unsigned short token_t;

typedef void(*usernode_op_out)(void *data);
typedef void(*usernode_op_datadelete)(void *data);
typedef void(*usernode_op_configdelete)(void *config);
/* suffix tree user data */
typedef struct {
	usernode_op_out out;
	usernode_op_datadelete datadelete;
	usernode_op_configdelete configdelete;
} st_usernode_op_s;
typedef struct {
	int flag;
	const char *tag;
	void *data;
	void *config;
	st_usernode_op_s *op;
} suffixtree_usernode_s;

/* suffix node user data */
typedef struct {
	usernode_op_out out;
	usernode_op_datadelete datadelete;
} sn_usernode_op_s;
typedef struct {
	int flag;
	const char *tag;
	void *data;
	sn_usernode_op_s *op;
} suffixnode_usernode_s;

/* suffix text matrix */
typedef struct {
	token_t **matrix;
	size_t cidx; /* column index */
	size_t ridx; /* row index */
	size_t row_size; /* row size */
	size_t column_size; /* column size */
} textmatrix_s;

/* suffix link */
typedef struct _suffix_link {
	struct _suffix_link *next;
	struct _suffix_node *suffix_node;
	size_t sidx;
	size_t eidx;
	size_t string_id;
	size_t substring_id;
} suffix_link_s;

/* suffix node */
typedef struct _suffix_node {
	size_t sidx; /* start index */
	size_t eidx; /* end index */
	struct _suffix_node *parent;
	struct _suffix_node **edges;
	int edgetotal;
	int edgesize;
	suffix_link_s **tracer;
	int tracertotal;
	int tracersize;
	size_t string_id;
	size_t substring_id;
	list_s user_list;
	struct _suffix_node *resource_chained;
} suffix_node_s;

/* suffix tree */
typedef struct {
	suffix_node_s *root;
	suffix_node_s *resource_head;
	suffix_link_s *suffix_link_head;
	suffix_link_s *suffix_link_tail;
	suffix_link_s *suffix_leaf_head;
	suffix_link_s *suffix_link_cache;
	textmatrix_s *matrix;
	int use_node_cache; /* whether use cache */
	int node_cache_size;
	int use_edge_cache; /* whether use edge cache */
	int suffixlink_size;
	int leaf_size;
	size_t string_id;
	size_t substring_id;
	size_t token_position;
	size_t change_number; /* op: insert */
	suffixtree_encode_e encode;
	list_s user_list;
} suffix_tree_s;

/* Suffix Tree */
suffix_tree_s *_suffixtree_create();
void _suffixtree_clean(suffix_tree_s *suffixtree);
void _suffixtree_use_node_cache(suffix_tree_s *suffixtree, int use);
int _suffixtree_is_use_node_cache(suffix_tree_s *suffixtree);
void _suffixtree_insert(suffix_tree_s *suffixtree, string16 *s);
void _suffixtree_change(suffix_tree_s *suffixtree);
size_t _suffixtree_change_number(suffix_tree_s *suffixtree);

/* Suffix Node */
suffix_node_s *_suffixnode_head_node(suffix_tree_s *suffixtree, suffix_node_s *node);

/* Text Matrix */
string16 *textmatrix_getstr(suffix_tree_s *suffixtree, string16 *s, size_t start, size_t end);

/* Config */
void _suffixtree_set_encode(suffix_tree_s *suffixtree, suffixtree_encode_e encode);
suffixtree_encode_e _suffixtree_get_encode(suffix_tree_s *suffixtree);

/* Suffix Tree UserNode */
#define SUFFIXTREE_USERNODE_ZERO 0x0000
#define SUFFIXTREE_USERNODE_DELETE_EXIST 0x001
suffixtree_usernode_s *_suffixtree_set_usernode(suffix_tree_s *suffixtree, const char *tag, void *userdata, void *config, st_usernode_op_s *op, int flag);
void _suffixtree_usernode_set_flag(suffixtree_usernode_s *usernode, int flag);
suffixtree_usernode_s *_suffixtree_get_usernode(suffix_tree_s *suffixtree, char *tag);
void *_suffixtree_get_userdata(suffix_tree_s *suffixtree, char *tag);
void _suffixtree_clear_usernode(suffix_tree_s *suffixtree, char *tag);

/* Suffix Node UserNode */
#define SUFFIXNODE_USERNODE_ZERO 0x0000
#define SUFFIXNODE_USERNODE_DELETE_EXIST 0x001
suffixnode_usernode_s *_suffixnode_set_usernode(suffix_node_s *suffixnode, const char *tag, void *userdata, sn_usernode_op_s *op, int flag);
void _suffixnode_usernode_set_flag(suffixnode_usernode_s *usernode, int flag);
suffixnode_usernode_s *_suffixnode_get_usernode(suffix_node_s *suffixnode, char *tag);
void *_suffixnode_get_userdata(suffix_node_s *suffixnode, char *tag);
void _suffixnode_clear_usernode(suffix_node_s *suffixnode, char *tag);

/* Tree Self Operate */
#define suffixnode_each_edge(node, pater) {int dss;for(dss=0;dss<pater->edgesize;++dss){(node=pater->edges[dss]);
#define suffixnode_each_end }}

#endif /* _SUFFIX_TREE_H_ */
