#ifndef AVLTREE_H
#define AVLTREE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

// copy from <linux/kernel.h> and strip debug stuffs.
#define container_of(ptr, type, member) ({			\
	void *__mptr = (void *)(ptr);					\
	((type *)(__mptr - offsetof(type, member))); })

struct avlnode;
/*
eval:          a <cmp op> b
ret val    <---  -1| 0 | 1  ---> (int)
meaning    <---  LT|EQL|GT  --->

ex:
    cmp(a, b) -> -1  equal to a < b
*/
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

#define _STMT(stmt) \
do {                \
    (stmt)          \
} while (0);

#define avl_tree_decl(sym, comparator)  \
struct avltree (sym) = {                \
    .root   = NULL,                     \
    .cmp    = &(comparator),            \
    .count  = 0,                        \
}

#define avl_tree_init(tree, comparator) \
_STMT(                                  \
    (tree)->root    = NULL;             \
    (tree)->cmp     = &(cmparator);     \
    (tree)->count   = 0;                \
)

#define avl_node_init(node)                 \
_STMT(                                      \
    memset((node), 0, sizeof(struct avlnode));\
)


struct avlnode *avl_insert(struct avltree *tree, struct avlnode *node);
struct avlnode *avl_get(struct avltree *tree, struct avlnode *hint);
struct avlnode *avl_delete(struct avltree *tree, struct avlnode *node);
struct avlnode *avl_replace(
    struct avltree *tree, struct avlnode *node, struct avlnode *sub
);

// debug use
void avl_print(struct avltree *tree);
void avl_validate(struct avltree *tree);

#endif /* AVLTREE_H */