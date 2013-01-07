/******************************************************* 
 * sirius
 * Created: 2009-5-22 @ error.d
 *******************************************************/

#ifndef _MODULETREE_H_
#define _MODULETREE_H_

#include <wordgroup.h>

#define MODULE_NAME_SIZE 1024

typedef enum {
	MNTYPE_HEAD = 1,
	MNTYPE_MODULE,
	MNTYPE_FILTER,
	MNTYPE_MERGER
} mntype_e;

struct _modulenode {
	char module_name[MODULE_NAME_SIZE];
	mntype_e type;
	wordgroup_t *out;
	void (*process)(struct _modulenode *mnode);
	struct _modulenode **parent;
	int parentsize;
	struct _modulenode **child;
	int childsize;
	struct _modulenode *chain; /* resource chain */
};
typedef struct _modulenode modulenode_t;
typedef void (*process_f)(modulenode_t *mnode);

modulenode_t *mnode_new(const char *modname, mntype_e mntype, process_f process);
void mnode_link(modulenode_t *parent, modulenode_t *child);
void mnode_free(modulenode_t *node); 
wordgroup_t *monde_input(modulenode_t *node, int idx);
void mnode_out(modulenode_t *node, wordgroup_t *out);

/* module tree */
struct _moduletree {
	modulenode_t *root;
	modulenode_t *resource; /* resource chain head */
	wordgroup_t *input;
	wordgroup_t *out;
};
typedef struct _moduletree moduletree_t;

moduletree_t *modtree_new();
void modtree_free(moduletree_t *modtree);
void modtree_set_input(moduletree_t *modtree, wordgroup_t *input);
wordgroup_t *modtree_get_out(moduletree_t *modtree);
/* new module node, need add resource to module tree */
void modtree_addresource(moduletree_t *mtree, modulenode_t *node);
void modtree_run(moduletree_t *modtree); 

#endif /* _MODULETREE_H_ */

