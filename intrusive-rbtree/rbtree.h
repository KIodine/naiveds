#ifndef RBTREE_H
#define RBTREE_H

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

/* or just include `linux/kernel.h`? */
#define container_of(ptr, type, member) ({      \
    void *__mptr = (void*)(ptr);                \
    (type*)(__mptr - offsetof(type, member));   \
})
#define rbt_is_black(node)  ((node)->color == COLOR_BLACK)
#define rbt_is_red(node)    ((node)->color == COLOR_RED)
#define rbt_set_black(node) ((node)->color = COLOR_BLACK)
#define rbt_set_red(node)   ((node)->color = COLOR_RED)
#define rbt_cpy_color(a, b) ((a)->color = (b)->color)


struct rbtnode {
    int color;
    int key, val;
    struct rbtnode *left, *right, *parent;
};

typedef int (*rbt_cmp_func)(struct rbtnode*, struct rbtnode*);

struct rbtree {
    unsigned int count;
    rbt_cmp_func cmp;
    struct rbtnode *root, *nil;
};

enum {
    RBT_OK,
    RBT_NOELEM,
    RBT_EXISTED,
};

enum {
    COLOR_RED,
    COLOR_BLACK,
};

enum {
    CMP_LT = -1,
    CMP_GT =  1,
    CMP_EQ =  0,
};

void            rbtree_init(struct rbtree *tree, struct rbtnode *nil,
                            rbt_cmp_func cmp);
int             rbtree_set(struct rbtree *tree, struct rbtnode *node);
struct rbtnode *rbtree_get(struct rbtree *tree, struct rbtnode *hint);
void            rbtree_delete(struct rbtree *tree, struct rbtnode *node);

int             rbtree_validate(struct rbtree *tree);

/* new API
struct rbtnode *rbtree_min(struct rbtree *tree);
struct rbtnode *rbtree_next(struct rbtnode *node);
*/


#endif