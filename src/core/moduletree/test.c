#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <moduletree.h>

static int n = 0;

void process(modulenode_t *node)
{
	n++;
}


int main()
{
	int i = 5;
	modulenode_t *node, *next;
	moduletree_t *tree;

	tree = modtree_new();
	
	while (i--) {
		next = mnode_new("node", MNTYPE_MODULE, process);
		
		
	}
	printf("%d\n", n);
	return 0;
}

