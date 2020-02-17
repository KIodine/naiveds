#ifndef AVLTREE_H
#define AVLTREE_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

// copy from <linux/kernel.h> and strip debug stuffs.
#define container_of(ptr, type, member) ({			\
	void *__mptr = (void *)(ptr);					\
	((type *)(__mptr - offsetof(type, member))); })


typedef int (*avl_cmp_t)(struct avlnode const *a, struct avlnode const *b);

struct avlnode {
    struct avlnode *child[2], *parent;
    int factor;
};

struct avltree {
    struct avlnode *root;
    int count;
    avl_cmp_t cmp;
};

enum {
    // type generic comp res
    NODE_LT = -1,
    NODE_EQ =  0,
    NODE_GT =  1,
    // access child pointer
    CLD_L   =  0,
    CLD_R   =  1,
};


void avl_tree_init(struct avltree *tree, avl_cmp_t cmp);
void avl_node_init(struct avlnode *node);

int avl_insert(struct avltree *tree, struct avlnode *node);
struct avlnode *avl_get(struct avltree *tree, struct avlnode *hint);
int avl_delete(struct avltree *tree, struct avlnode *node);


#endif