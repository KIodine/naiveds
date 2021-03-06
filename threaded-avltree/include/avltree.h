#ifndef AVLTREE_H
#define AVLTREE_H

#include <string.h>
#include <stddef.h>

#include "list.h"

/*  TODO:
    - Implement `threaded-avltree`.
        - Add a `struct list node` member.
        - Add implementation of insert in `avl_insert`.
        - Add implementation of remove in `avl_delete`.
        - Update `avl_replace`.
        - Add `avl_iter` macro for iteration.
        - Add `avl_min` and `avl_max` inline functions.
*/

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
    struct list     node;
    int factor;
};

struct avltree {
    struct avlnode *root;
    struct list     head;
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

static inline void avl_tree_init(struct avltree *tree, avl_cmp_t cmp){
    tree->root  = NULL;
    tree->cmp   = cmp;
    tree->count = 0;
    tree->head.prev = &tree->head;
    tree->head.next = &tree->head;
    return;
};

#define avl_node_init(node)                 \
_STMT(                                      \
    memset((node), 0, sizeof(struct avlnode));\
)

static inline struct avlnode *avl_get_min(struct avltree *tree){
    return container_of(tree->head.next, struct avlnode, node);
}

static inline struct avlnode *avl_get_max(struct avltree *tree){
    return container_of(tree->head.prev, struct avlnode, node);
}


struct avlnode *avl_insert(struct avltree *tree, struct avlnode *node);
struct avlnode *avl_get(struct avltree *tree, struct avlnode *hint);
struct avlnode *avl_delete(struct avltree *tree, struct avlnode *node);
struct avlnode *avl_replace(
    struct avltree *tree, struct avlnode *node, struct avlnode *sub
);


#endif /* AVLTREE_H */