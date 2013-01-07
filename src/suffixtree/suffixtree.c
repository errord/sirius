#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <suffixtree.h>


/* function */
static void suffixnode_add_tracer(suffix_node_s *node, suffix_link_s *tracer);
static void suffixnode_del_tracer(suffix_node_s *node, suffix_link_s *tracer);
/* function end */


#define coord2position(textmatrix, c, r) (c * textmatrix->row_size + r)
#define position2column(textmatrix, position) (position / textmatrix->row_size)
#define position2row(textmatrix, position) (position % textmatrix->row_size)


/* suffix tree user node resource free */
static void _suffixtree_usernode_free(void *node)
{
	suffixtree_usernode_s *usernode = (suffixtree_usernode_s*)node;
	if (usernode->data) {
		if (usernode->op->datadelete)
			usernode->op->datadelete(usernode->data);
	}
	if (usernode->config) {
		if (usernode->op->configdelete)
			usernode->op->configdelete(usernode->config);
	}
	
	free(usernode);
}

/* suffix node user node resource free */
static void _suffixnode_usernode_free(void *node)
{
	suffixnode_usernode_s *usernode = (suffixnode_usernode_s*)node;
	if (usernode->data) {
		if (usernode->op->datadelete)
			usernode->op->datadelete(usernode->data);
	}
	free(usernode);
}

static textmatrix_s *textmatrix_create(size_t row, size_t column)
{
	textmatrix_s *textmatrix;
	size_t i;
	if (row == 0)
		row = TEXTMATRIX_DEFAULT_ROW_SIZE;
	if (column == 0)
		column = TEXTMATRIX_DEFAULT_COLUMN_SIZE;
	textmatrix = (textmatrix_s*)malloc(sizeof(textmatrix_s));
	textmatrix->ridx = 0;
	textmatrix->cidx = 0;
	textmatrix->row_size = row;
	textmatrix->column_size = column;
	textmatrix->matrix = (token_t**)malloc(sizeof(token_t*) * column);
	/* init matrix */
	for (i = 0; i < column; ++i) {
		textmatrix->matrix[i] = (token_t*)malloc(sizeof(token_t) * row);
		memset(textmatrix->matrix[i], 0, sizeof(token_t) * row);
	}
	return textmatrix;
}

static void textmatrix_clean(textmatrix_s *textmatrix)
{
	size_t c;
	for (c = 0; c < textmatrix->column_size; ++c)
		if (textmatrix->matrix[c] != NULL)
			free(textmatrix->matrix[c]);
	free(textmatrix->matrix);
	free(textmatrix);
}

static void textmatrix_realloc(textmatrix_s *textmatrix, size_t column)
{
	token_t **matrix;
	size_t c;
	matrix = (token_t**)malloc(sizeof(token_t*) * column);
	memset(matrix, 0, sizeof(token_t*) * column);
	/* copy old to new matrix */
	for (c = 0; c < textmatrix->column_size; ++c)
		matrix[c] = textmatrix->matrix[c];
	/* alloc new row */
	for (; c < column; ++c) {
		matrix[c] = (token_t*)malloc(sizeof(token_t) * textmatrix->row_size);
		memset(matrix[c], 0, sizeof(token_t) * textmatrix->row_size);
	}
	/* release old matrix */
	free(textmatrix->matrix);
	textmatrix->matrix = matrix;
	textmatrix->column_size = column;
}

/*
 * put token to matrix
 * return token position
 */
static size_t textmatrix_puttoken(textmatrix_s *textmatrix, token_t token)
{
	size_t cidx,ridx;
	cidx = textmatrix->cidx;
	ridx = textmatrix->ridx + 1;
	if (ridx >= textmatrix->row_size) {
		cidx++;
		if (cidx >= textmatrix->column_size) {
			size_t size = textmatrix->column_size;
			size *= 2;
			textmatrix_realloc(textmatrix, size);
		}
		ridx = 0;
	}
	textmatrix->matrix[cidx][ridx] = token;
	textmatrix->cidx = cidx;
	textmatrix->ridx = ridx;
	return coord2position(textmatrix, cidx, ridx);
}

#ifdef GETTOKEN_OPTIMIZE
#define textmatrix_gettoken_optimize(token, textmatrix, position)	\
	token = textmatrix->matrix[position2column(textmatrix, position)][position2row(textmatrix, position)]
#define GET_TOKEN(token, textmatrix, position) textmatrix_gettoken_optimize(token, textmatrix, position)
#else
static token_t textmatrix_gettoken_no_optimize(textmatrix_s *textmatrix, size_t position)
{
	size_t cidx,ridx;
	cidx = position2column(textmatrix, position);
	ridx = position2row(textmatrix, position);
	return textmatrix->matrix[cidx][ridx];
}
#define GET_TOKEN(token, textmatrix, position) token = textmatrix_gettoken_no_optimize(textmatrix, position)
#endif /* GETTOKEN_OPTIMIZE */


/*
 * get token from matrix
 * return token
 */
#define textmatrix_gettoken(token, textmatrix, position) GET_TOKEN(token, textmatrix, position)

/*
 * suffix link cache use rate
 */
static int suffixlink_cache_rate(suffix_tree_s *suffixtree)
{
	return (suffixtree->node_cache_size / (suffixtree->suffixlink_size + suffixtree->leaf_size));
}

/*
 * return 1 save pick node to cache, other return 0
 */
static int suffixlink_is_save_cache(suffix_tree_s *suffixtree)
{
	/* cache is used 2 times, do not save */
	if (suffixlink_cache_rate(suffixtree) >= 2)
		return 0;
	else
		return 1;
}

/*
 * get link node
 * get link node from suffix link cache or new suffix link node
 */
static suffix_link_s *suffixlink_get_node(suffix_tree_s *suffixtree, suffix_node_s *suffix_node, size_t sidx, size_t eidx, size_t string_id, size_t substring_id, suffix_link_s *next)
{
	suffix_link_s *link_node;
	/* get link node */
	if (suffixtree->use_node_cache && suffixtree->suffix_link_cache != NULL) {
		link_node = suffixtree->suffix_link_cache;
		suffixtree->suffix_link_cache = link_node->next;
		suffixtree->node_cache_size--;
	} else {
		link_node = (suffix_link_s*)malloc(sizeof(suffix_link_s));
	}
	link_node->suffix_node = suffix_node;
	link_node->sidx = sidx;
	link_node->eidx = eidx;
	link_node->string_id = string_id;
	link_node->substring_id = substring_id;
	link_node->next = next;
	return link_node;
}

/*
 * suffixlink node link to cache
 */
static void suffixlink_link_to_cache(suffix_tree_s *suffixtree, suffix_link_s *link_node)
{
	link_node->suffix_node = NULL;
	link_node->sidx = 0;
	link_node->eidx = 0;
	link_node->string_id = 0;
	link_node->substring_id = 0;
	link_node->next = suffixtree->suffix_link_cache;
	suffixtree->suffix_link_cache = link_node;
	suffixtree->node_cache_size++;	
}

/*
 * suffix link node list move or del
 * cache is use, all leaf node move cache
 * cache not use, all leaf node delete
 */
static void suffixlink_list_empty(suffix_tree_s *suffixtree, suffix_link_s *link_node)
{
	suffix_link_s *next;
	while (link_node) {
		next = link_node->next;
		if (suffixtree->use_node_cache)
			suffixlink_link_to_cache(suffixtree, link_node);
		else
			free(link_node);
		link_node = next;
	}
}

/*
 * add link node
 * 1. get link node from suffix link cache or new suffix link node
 * 2. link to suffix leaf chain
 */
static suffix_link_s *suffix_leaf_add_node(suffix_tree_s *suffixtree, suffix_node_s *suffix_node)
{
	suffix_link_s *link_node;
	/* get link node */
	link_node = suffixlink_get_node(suffixtree, suffix_node, 0, 0, 0, 0, NULL);
	
	/* link to leaf chain */
	if (suffixtree->suffix_leaf_head == NULL) { /* head node */
		suffixtree->suffix_leaf_head = link_node;
	} else {
		link_node->next = suffixtree->suffix_leaf_head;
		suffixtree->suffix_leaf_head = link_node;
	}
	suffixtree->leaf_size++;
	return link_node;
}

/*
 * add link node
 * 1. link to suffix link
 * 2. add suffix node tracer
 */
static suffix_link_s *suffixlink_add_linknode(suffix_tree_s *suffixtree, suffix_node_s *suffix_node, size_t sidx, size_t eidx, size_t string_id, size_t substring_id)
{
	suffix_link_s *link_node;
	/* get link node */
	link_node = suffixlink_get_node(suffixtree, suffix_node, sidx, eidx, string_id, substring_id, NULL);
	
	/* link to suffix link */
	if (suffixtree->suffix_link_head == NULL) { /* head node */
		suffixtree->suffix_link_head = link_node;
		suffixtree->suffix_link_tail = link_node;
	} else {
		suffixtree->suffix_link_tail->next = link_node;
		suffixtree->suffix_link_tail = link_node;
	}
	/* add suffix node tracer */
	suffixnode_add_tracer(suffix_node, link_node);

	suffixtree->suffixlink_size++;
	return link_node;
}

/*
 * pick suffix link node
 * 1. pick link node from suffix link chain
 * 2. delete suffix node tracer
 * 3. link to suffix link cache or delete
 */
static void suffixlink_pick_node(suffix_tree_s *suffixtree, suffix_link_s *prev_node, suffix_link_s *link_node)
{
	/* pick node */
	if (prev_node == NULL) { /* head node */
		suffixtree->suffix_link_head = link_node->next;
		if (suffixtree->suffix_link_tail == link_node) { /* head and tail node */
			suffixtree->suffix_link_tail = NULL;
		}
	} else { /* middle and tail node */
		prev_node->next = link_node->next;
	}
	suffixtree->suffixlink_size--;
	
	/* delete tracer */
	suffixnode_del_tracer(link_node->suffix_node, link_node);

	/* whether save */
	if (suffixtree->use_node_cache && suffixlink_is_save_cache(suffixtree))
		suffixlink_link_to_cache(suffixtree, link_node);
	else
		free(link_node);
}

/*
 * empty suffix leaf chain
 * cache is use, all leaf node move cache
 * cache not use, all leaf node delete
 */
static void suffix_leaf_chain_empty(suffix_tree_s *suffixtree)
{
	suffixlink_list_empty(suffixtree, suffixtree->suffix_leaf_head);
	suffixtree->leaf_size = 0;
	suffixtree->suffix_leaf_head = NULL;
}

/*
 * empty suffix link
 * cache is use, all suffix link node move cache
 * cache not use, all suffix link node delete
 */
static void suffixlink_empty(suffix_tree_s *suffixtree)
{
	suffixlink_list_empty(suffixtree, suffixtree->suffix_link_head);
	suffixtree->suffixlink_size = 0;
	suffixtree->suffix_link_head = NULL;
	suffixtree->suffix_link_tail = NULL;
}

/*
 * clean suffix link chain
 * 1. clean suffix_link
 * 2. clean suffix_link_cache
 * 3. clean suffix_leaf
 */
static void suffixlink_clean(suffix_tree_s *suffixtree)
{
#define SUFFIX_LINK_CHAIN_CLEAN			\
	while (link_node) {			\
		next = link_node->next;		\
		free(link_node);		\
		link_node = next;		\
	}

	suffix_link_s *link_node, *next;
	/* clean suffix link */
	link_node = suffixtree->suffix_link_head;
	SUFFIX_LINK_CHAIN_CLEAN;
	/* clean suffix link cache */
	link_node = suffixtree->suffix_link_cache;
	SUFFIX_LINK_CHAIN_CLEAN;
	/* clean suffix leaf chain */
	link_node = suffixtree->suffix_leaf_head;
	SUFFIX_LINK_CHAIN_CLEAN;

	suffixtree->suffix_link_head = NULL;
	suffixtree->suffix_link_tail = NULL;
	suffixtree->suffix_link_cache = NULL;
	suffixtree->suffix_leaf_head = NULL;
	suffixtree->node_cache_size = 0;
	suffixtree->suffixlink_size = 0;
	suffixtree->leaf_size = 0;
}

/*
 * create suffix node
 */
static suffix_node_s *suffixnode_create(suffix_tree_s *suffixtree, suffix_node_s *parent, size_t string_id, size_t substring_id)
{
	suffix_node_s *node;
	node = (suffix_node_s*)malloc(sizeof(suffix_node_s));
	node->sidx = 0;
	node->eidx = 0;
	node->parent = parent;
	node->edges = NULL;
	node->edgetotal = 0;
	node->edgesize = 0;
	node->tracer = NULL;
	node->tracertotal = 0;
	node->tracersize = 0;
	node->string_id = string_id;
	node->substring_id = substring_id;
	list_resource_init(&(node->user_list), _suffixnode_usernode_free);
	/* link to resource chained */
	node->resource_chained = suffixtree->resource_head;
	suffixtree->resource_head = node;
	return node;
}

static void suffixnode_clean(suffix_node_s *node)
{
	if (node->edges != NULL)
		free(node->edges);
	if (node->tracer != NULL)
		free(node->tracer);
	list_resource_clear(&(node->user_list));
	free(node);
}

static void suffixnode_add_edge(suffix_node_s *node, suffix_node_s *edge)
{
	int idx = node->edgesize;
	node->edgesize++;
	if (node->edgesize > node->edgetotal) {
		node->edgetotal = node->edgesize * 2;
		node->edges = realloc(node->edges, sizeof(suffix_node_s) * node->edgetotal);
	}
	node->edges[idx] = edge;
}

static int suffixnode_replace_edge(suffix_node_s *node, suffix_node_s *edge, suffix_node_s *new_edge)
{
	int i;
	for (i = 0; i < node->edgesize; ++i) {
		if (node->edges[i] == edge) {
			node->edges[i] = new_edge;
			return 1;
		}
	}
	return 0;
}

static void suffixnode_add_tracer(suffix_node_s *node, suffix_link_s *tracer)
{
	node->tracersize++;
	if (node->tracersize > node->tracertotal) {
		node->tracertotal = node->tracersize * 2;
		node->tracer = realloc(node->tracer, sizeof(suffix_link_s) * node->tracertotal);
	}
	node->tracer[node->tracersize-1] = tracer;
}

static void suffixnode_del_tracer(suffix_node_s *node, suffix_link_s *tracer)
{
	int i;
	/* find tracer */
	for (i = 0; i < node->tracersize; ++i)
		if (node->tracer[i] == tracer)
			break;
	/* move */
	for (++i; i < node->tracersize; ++i)
		node->tracer[i-1] = node->tracer[i];
	node->tracersize--;
}

/*
 * move not less limit tracer of src node to des node
 */
static void suffixnode_move_tracer(suffix_node_s *src, suffix_node_s *des, size_t limit)
{
	suffix_link_s **tracer;
	suffix_link_s *link_node;
	int idx = 0, i;
	assert(src->tracersize >= 0);
	if (src->tracersize == 0)
		return ;
	tracer = (suffix_link_s**)malloc(sizeof(suffix_link_s*) * src->tracertotal);
	for (i = 0; i < src->tracersize; ++i) {
		link_node = src->tracer[i];
		if ((link_node->eidx - link_node->sidx) <= limit) {
			/* move to des*/
			suffixnode_add_tracer(des, link_node);
			link_node->suffix_node = des;
		} else {
			/* move new tracer array */
			tracer[idx] = link_node;
			idx++;
		}
	}

	/* replace src node tracer array */
	free(src->tracer);
	src->tracer = tracer;
	src->tracersize = idx;
}

/*
 * whether include
 * return 1 is include
 * return 0 is no include
 */
static int suffixtree_node_include(suffix_tree_s *suffixtree, suffix_link_s *link_node, suffix_node_s *suffix_node)
{
	token_t lt,nt;
	size_t offset, l_pos, n_pos;
	offset = link_node->eidx - link_node->sidx;
	l_pos = link_node->sidx + offset;
	n_pos = suffix_node->sidx + offset;
	textmatrix_gettoken(lt, suffixtree->matrix, l_pos);
	textmatrix_gettoken(nt, suffixtree->matrix, n_pos);
	if (lt == nt && lt != 0)
		return 1;
	return 0;
}

/*
 * split leaf node
 */
static void suffixtree_split_leaf_node(suffix_tree_s *suffixtree, suffix_link_s *link_prev, suffix_link_s *link_node, suffix_node_s *parent, size_t string_id, size_t substring_id)
{
	suffix_node_s *leaf;
	leaf = suffixnode_create(suffixtree, parent, string_id, substring_id);
	leaf->sidx = link_node->eidx;
	suffixnode_add_edge(parent, leaf);
	suffix_leaf_add_node(suffixtree, leaf);
	suffixlink_pick_node(suffixtree, link_prev, link_node);
}

/*
 * check suffix node
 * return 1 if split
 * return 0 if no split
 */
static int suffixtree_check_node(suffix_tree_s *suffixtree, suffix_link_s *prev_node, suffix_link_s *link_node)
{
	suffix_node_s *suffix_node, *middle_node;
	size_t link_range, suffix_range, seidx;
	int i;
	int exist = 0;
	suffix_node = link_node->suffix_node;
	seidx = suffix_node->eidx != 0 ? suffix_node->eidx : suffixtree->token_position;
	link_range = link_node->eidx - link_node->sidx;	
	suffix_range = seidx - suffix_node->sidx;
	/* range not match, find all edge */
	if (link_range > suffix_range) {
		/* add match range */
		link_node->sidx = link_node->sidx + suffix_range + 1;

		for (i = 0; i < suffix_node->edgesize; ++i) {
			if (suffixtree_node_include(suffixtree, link_node, suffix_node->edges[i]) == 1) {
				exist = 1;
				break;
			}
		}

		/* exist edge */
		if (exist) {
			link_node->suffix_node = suffix_node->edges[i];
			suffixnode_del_tracer(suffix_node, link_node);
			suffixnode_add_tracer(suffix_node->edges[i], link_node);
			return 0;			
		}
		
		/* split leaf node */
		suffixtree_split_leaf_node(suffixtree, prev_node, link_node, suffix_node, 
					   link_node->string_id, link_node->substring_id);
		return 1;
	}
	/* 
	 * no include 
	 */
	if (suffixtree_node_include(suffixtree, link_node, suffix_node) == 0) {
		/* 
		 * add middle node, replace current suffix node
		 * middle node no belong to any string 
		 */
		middle_node = suffixnode_create(suffixtree, suffix_node->parent, 0, 0);
		middle_node->sidx = suffix_node->sidx;
		middle_node->eidx = suffix_node->sidx + link_range - 1;
		suffix_node->sidx = middle_node->eidx + 1;
		suffixnode_replace_edge(suffix_node->parent, suffix_node, middle_node);
		suffixnode_add_edge(middle_node, suffix_node);
		suffix_node->parent = middle_node;

		/* split leaf node */
		suffixtree_split_leaf_node(suffixtree, prev_node, link_node, middle_node, 
					   link_node->string_id, link_node->substring_id);

		/* move tracer of suffix node to middle node */
		suffixnode_move_tracer(suffix_node, middle_node, middle_node->eidx - middle_node->sidx);

		return 1;
	}
	return 0;
}

/*
 * suffix link node trace
 * 1. trace all suffix link node
 * 2. whether split child node
 */
static void suffixtree_trace_suffixlink(suffix_tree_s *suffixtree)
{
	suffix_link_s *link_node, *next, *prev = NULL, *grandparent = NULL;
	link_node = suffixtree->suffix_link_head;
	while (link_node) {
		/* broaden link node range */
		link_node->eidx++;
		next = link_node->next;
		grandparent = prev;
		/* is split, link_node is empty */
		if (suffixtree_check_node(suffixtree, prev, link_node) == 1)
			prev = grandparent;
		else
			prev = link_node;
		link_node = next;
	}
}

/*
 * insert suffix node in root
 */
static void suffixtree_insert_suffixnode(suffix_tree_s *suffixtree)
{
	suffix_node_s *suffix_node = NULL, *leaf;
	token_t insert_token,node_token;
	size_t node_position;
	int i, exist = 0;
	textmatrix_gettoken(insert_token, suffixtree->matrix, suffixtree->token_position);
	for (i = 0; i < suffixtree->root->edgesize; ++i) {
		suffix_node = suffixtree->root->edges[i];
		node_position = suffix_node->sidx;
		textmatrix_gettoken(node_token, suffixtree->matrix, node_position);
		if (node_token == insert_token) {
			exist = 1;
			break;
		}
	}
	if (exist == 1) {
		suffixlink_add_linknode(suffixtree, suffix_node, 
					suffixtree->token_position, 
					suffixtree->token_position, 
					suffixtree->string_id, 
					suffixtree->substring_id);
	} else {
		/* add new leaf node */
		leaf = suffixnode_create(suffixtree, suffixtree->root, suffixtree->string_id, suffixtree->substring_id);
		leaf->sidx = suffixtree->token_position;
		suffixnode_add_edge(suffixtree->root, leaf);
		suffix_leaf_add_node(suffixtree, leaf);
	}
}

/*
 * close all leaf node from suffix leaf chain
 */
static void suffixtree_close_leaf_chain(suffix_tree_s *suffixtree)
{
	suffix_link_s *leaf_node;
	leaf_node = suffixtree->suffix_leaf_head;
	while (leaf_node) {
		leaf_node->suffix_node->eidx = suffixtree->token_position;
		leaf_node = leaf_node->next;
	}
	suffix_leaf_chain_empty(suffixtree);
}

/* API */

/*
 * create suffix tree
 * default cache be on
 */
suffix_tree_s *_suffixtree_create()
{
	suffix_tree_s *tree;
	assert(sizeof(token_t) == sizeof(_16b));
	tree = (suffix_tree_s*)malloc(sizeof(suffix_tree_s));
	tree->resource_head = NULL;
	tree->root = suffixnode_create(tree, NULL, 0, 0);
	tree->suffix_link_head = NULL;
	tree->suffix_link_tail = NULL;
	tree->suffix_link_cache = NULL;
	tree->suffix_leaf_head = NULL;
	tree->matrix = textmatrix_create(0, 0);
	tree->use_node_cache = 1;
	tree->node_cache_size = 0;
	tree->suffixlink_size = 0;
	tree->leaf_size = 0;
	tree->string_id = 0;
	tree->substring_id = 0;
	tree->token_position = 0;
	tree->change_number = 0;
	tree->encode = suffixtree_encode_unicode;
	list_resource_init(&(tree->user_list), _suffixtree_usernode_free);
	return tree;
};

/*
 * clean suffix tree
 */
void _suffixtree_clean(suffix_tree_s *suffixtree)
{
	suffix_node_s *node, *next;
	node = suffixtree->resource_head;
	while (node != NULL) {
		next = node->resource_chained;
		suffixnode_clean(node);
		node = next;
	}
	textmatrix_clean(suffixtree->matrix);
	suffixlink_clean(suffixtree);
	list_resource_clear(&(suffixtree->user_list));
	free(suffixtree);
}

void _suffixtree_use_node_cache(suffix_tree_s *suffixtree, int use)
{
	suffixtree->use_node_cache = use;
}

int _suffixtree_is_use_node_cache(suffix_tree_s *suffixtree)
{
	return suffixtree->use_node_cache;
}

void _suffixtree_insert(suffix_tree_s *suffixtree, string16 *s)
{
	token_t *token;
	size_t token_position;
	token = (token_t*)s16_getstr(s);
	suffixtree->string_id++;
	suffixtree->substring_id = 0;
	_suffixtree_change(suffixtree);
	/* online (line-time) create suffix tree */
	while (*token) {
		token_position = textmatrix_puttoken(suffixtree->matrix, *token);
		suffixtree->token_position = token_position;
		/* trace suffix link */
		suffixtree_trace_suffixlink(suffixtree);
		/* insert current token in root */
		suffixtree_insert_suffixnode(suffixtree);
		suffixtree->substring_id++;
		token++;
	}
	/* add end token */
	token_position = textmatrix_puttoken(suffixtree->matrix, (token_t)0);
	suffixtree->token_position = token_position;
	suffixtree_trace_suffixlink(suffixtree);
	assert(suffixtree->suffix_link_head == NULL);
	/* close leaf node */
	suffixtree_close_leaf_chain(suffixtree);
}

void _suffixtree_change(suffix_tree_s *suffixtree)
{
	suffixtree->change_number++;
}

size_t _suffixtree_change_number(suffix_tree_s *suffixtree)
{
	return suffixtree->change_number;
}


string16 *textmatrix_getstr(suffix_tree_s *suffixtree, string16 *s, size_t start, size_t end)
{
	token_t token;
	while (start <= end) {
		textmatrix_gettoken(token, suffixtree->matrix, start);
		s16_append16b(s, token);
		start++;
	}
	return s;
}

suffix_node_s *_suffixnode_head_node(suffix_tree_s *suffixtree, suffix_node_s *node)
{
	while (node->parent && node->parent != suffixtree->root) {
		node = node->parent;
	}
	return node;
}

void _suffixtree_set_encode(suffix_tree_s *suffixtree, suffixtree_encode_e encode)
{
	suffixtree->encode = encode;
}

suffixtree_encode_e _suffixtree_get_encode(suffix_tree_s *suffixtree)
{
	return suffixtree->encode;
}

static suffixtree_usernode_s *_suffixtree_find_tag(suffix_tree_s *suffixtree, const char *tag)
{
	suffixtree_usernode_s *usernode;
	list_each_data_m(usernode, suffixtree_usernode_s*, &(suffixtree->user_list)) {
		if (!strncmp(usernode->tag, tag, 12))
			return usernode;
	}
	list_each_data_end_m;
	return NULL;	
}

static suffixnode_usernode_s *_suffixnode_find_tag(suffix_node_s *suffixnode, const char *tag)
{
	suffixnode_usernode_s *usernode;
	list_each_data_m(usernode, suffixnode_usernode_s*, &(suffixnode->user_list)) {
		if (!strncmp(usernode->tag, tag, 12))
			return usernode;
	}
	list_each_data_end_m;
	return NULL;
}

suffixtree_usernode_s *_suffixtree_set_usernode(suffix_tree_s *suffixtree, const char *tag, void *userdata, void *config, st_usernode_op_s *op, int flag)
{
	suffixtree_usernode_s *usernode  = _suffixtree_find_tag(suffixtree, tag);
	if (usernode != NULL && (flag & SUFFIXTREE_USERNODE_DELETE_EXIST)) {
		if (usernode->data) {
			if (usernode->op->datadelete)
				usernode->op->datadelete(usernode->data);
		}
		usernode->tag = tag;
		usernode->flag |= flag;
		usernode->data = userdata;
		usernode->config = config;
		usernode->op = op;

	} else {	
		usernode = (suffixtree_usernode_s*)malloc(sizeof(suffixtree_usernode_s));
		usernode->tag = tag;
		usernode->flag |= flag;
		usernode->data = userdata;
		usernode->config = config;
		usernode->op = op;
		list_insert_tail(&(suffixtree->user_list), (void*)usernode);
	}
	
	return usernode;
}

void _suffixtree_usernode_set_flag(suffixtree_usernode_s *usernode, int flag)
{
	usernode->flag |= flag;
}

suffixtree_usernode_s *_suffixtree_get_usernode(suffix_tree_s *suffixtree, char *tag)
{
	suffixtree_usernode_s *usernode = _suffixtree_find_tag(suffixtree, tag);
	if (usernode != NULL)
		return usernode;
	return NULL;
}

void *_suffixtree_get_userdata(suffix_tree_s *suffixtree, char *tag)
{
	suffixtree_usernode_s *usernode = _suffixtree_find_tag(suffixtree, tag);
	if (usernode != NULL)
		return usernode->data;
	return NULL;
}

void _suffixtree_clear_usernode(suffix_tree_s *suffixtree, char *tag)
{
	suffixtree_usernode_s *usernode = _suffixtree_find_tag(suffixtree, tag);
	if (usernode != NULL)
		usernode->data = NULL;
}

suffixnode_usernode_s *_suffixnode_set_usernode(suffix_node_s *suffixnode, const char *tag, void  *userdata, sn_usernode_op_s * op, int flag)
{
	suffixnode_usernode_s *usernode  = _suffixnode_find_tag(suffixnode, tag);
	if (usernode != NULL && (flag & SUFFIXNODE_USERNODE_DELETE_EXIST)) {
		if (usernode->data) {
			if (usernode->op->datadelete)
				usernode->op->datadelete(usernode->data);
		}
		usernode->tag = tag;
		usernode->flag |= flag;
		usernode->data = userdata;
		usernode->op = op;
	} else {	
		usernode = (suffixnode_usernode_s*)malloc(sizeof(suffixnode_usernode_s));
		usernode->tag = tag;
		usernode->flag |= flag;
		usernode->data = userdata;
		usernode->op = op;
		list_insert_tail(&(suffixnode->user_list), (void*)usernode);
	}
	return usernode;
}

void _suffixnode_usernode_set_flag(suffixnode_usernode_s *usernode, int flag)
{
	usernode->flag |= flag;
}

suffixnode_usernode_s *_suffixnode_get_usernode(suffix_node_s *suffixnode, char *tag)
{
	suffixnode_usernode_s *usernode = _suffixnode_find_tag(suffixnode, tag);
	if (usernode != NULL)
		return usernode;
	return NULL;	
}

void *_suffixnode_get_userdata(suffix_node_s *suffixnode, char *tag)
{
	suffixnode_usernode_s *usernode = _suffixnode_find_tag(suffixnode, tag);
	if (usernode != NULL)
		return usernode->data;
	return NULL;	
}


void _suffixnode_clear_usernode(suffix_node_s *suffixnode, char *tag)
{
	suffixnode_usernode_s *usernode = _suffixnode_find_tag(suffixnode, tag);
	if (usernode != NULL)
		usernode->data = NULL;
}

