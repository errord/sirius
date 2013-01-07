/******************************************************* 
 * sirius
 * Created: 2009-5-22 @ error.d
 *******************************************************/

#include <stdlib.h>
#include <string.h>
#include <moduletree.h>

modulenode_t *mnode_new(const char *modname, mntype_e mntype, process_f process)
{
	modulenode_t *mnode;
	mnode = (modulenode_t*)malloc(sizeof(modulenode_t));
	strncpy(mnode->module_name, modname, MODULE_NAME_SIZE);
	mnode->type = mntype;
	mnode->process = process;
	mnode->out = NULL;
	mnode->parent = NULL;
	mnode->child = NULL;
	mnode->chain = NULL;
	mnode->parentsize = 0;
	mnode->childsize = 0;
	return mnode;
}

static modulenode_t **reallocnodelist(modulenode_t **oldptr, size_t size)
{
	int i;
	modulenode_t **newptr;
	newptr = (modulenode_t**)malloc(sizeof(modulenode_t*) * size);
	if (oldptr != NULL) {
		memcpy(newptr, oldptr, sizeof(modulenode_t*) * (size-1));
		free(oldptr);
	}
	return newptr;
}

void mnode_link(modulenode_t *parent, modulenode_t *child)
{
	parent->childsize++;
	child->parentsize++;
	parent->child = reallocnodelist(parent->child, parent->childsize);
	child->parent = reallocnodelist(child->parent, child->parentsize);
	parent->child[parent->childsize-1] = child;
	child->parent[child->parentsize-1] = parent;
}

void mnode_free(modulenode_t *node)
{
	wg_free(node->out);
	if (node->parent != NULL)
		free(node->parent);
	if (node->child != NULL)
		free(node->child);
	free(node);
}

wordgroup_t *monde_input(modulenode_t *node, int idx)
{
	if (node->parentsize == 0 || node->parentsize >= idx)
		return NULL;
	return node->parent[idx]->out;
}

void mnode_out(modulenode_t *node, wordgroup_t *out)
{
	node->out = out;
}

moduletree_t *modtree_new()
{
	moduletree_t *modtree;

	modtree = (moduletree_t*)malloc(sizeof(moduletree_t));
	modtree->root = NULL;
	modtree->resource = NULL;
	modtree->input = NULL;
	modtree->out = NULL;
	return modtree;
}

void modtree_free(moduletree_t *modtree)
{
	modulenode_t *node;
	node = modtree->resource;
	while (node) {
		node = node->chain;
		mnode_free(node);
	}
}

inline void modtree_set_input(moduletree_t *modtree, wordgroup_t *input)
{
	modtree->input = input;
}

inline wordgroup_t *modtree_get_out(moduletree_t *modtree)
{
	return modtree->out;
}

void modtree_addresource(moduletree_t *mtree, modulenode_t *node)
{
	node->chain = mtree->resource;
	mtree->resource = node;
}

static void module_process(modulenode_t *modnode)
{
	modnode->process(modnode);
}

static void filter_process(modulenode_t *modnode)
{
	modnode->process(modnode);
}

static void merge_process(modulenode_t *modnode)
{
	modnode->process(modnode);
}

static int allset(modulenode_t *modnode)
{
	modulenode_t **p;
	modulenode_t *node;
	int ps,i;
	p = modnode->parent;
	ps = modnode->parentsize;
	for (i = 0; i < ps; ++i) {
		node = p[i];
		if (node->out == NULL)
			return 0;
	}
	return 1;
}

static modulenode_t *_run(moduletree_t *modtree, modulenode_t *modnode)
{
	int next = 1;
	modulenode_t **child;
	int i;
	switch (modnode->type) {
	case MNTYPE_HEAD:
	case MNTYPE_MODULE:
		module_process(modnode);
		break;

	case MNTYPE_FILTER:
		filter_process(modnode);
		break;

	case MNTYPE_MERGER:
		if (!allset(modnode)) {
			next = 0;
			break;
		}
		merge_process(modnode);
		break;
	}
	if (next == 1) {
		for (i = 0; i < modnode->childsize; ++i) {
			_run(modtree, modnode->child[i]);
		}
	}
}

static void _setheadnode(moduletree_t *modtree, modulenode_t *head)
{
	strcpy(head->module_name, "head_module");
	head->type = MNTYPE_HEAD;
	head->process = NULL;
	head->parent = NULL;
	head->chain = NULL;
	mnode_link(head, modtree->root);
	head->out = modtree->input;
}

static void _freeheadnode(modulenode_t *head)
{
	free(head->child);
}

void modtree_run(moduletree_t *modtree)
{
	modulenode_t head;
	modulenode_t *tail;
	_setheadnode(modtree, &head);
	tail = _run(modtree, modtree->root);
	_freeheadnode(&head);
	modtree->out = tail->out;
}

