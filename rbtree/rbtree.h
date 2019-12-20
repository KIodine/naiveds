#ifndef RBTREE_H
#define RBTREE_H

#include <stdlib.h>
#include <assert.h>


struct rbtree {
    unsigned int count;
    struct rbtnode *root, *nil;
};

struct rbtnode {
    int color;
    int key, val;
    struct rbtnode *left, *right, *parent;
};


enum {
    RBT_OK,
    RBT_NOELEM,
};

enum {
    COLOR_RED,
    COLOR_BLACK,
};

struct rbtree *rbtree_alloc(void);
void rbtree_free(struct rbtree *tree);
int rbtree_set(struct rbtree *tree, int key, int val);
int rbtree_get(struct rbtree *tree, int key, int *res);
int rbtree_delete(struct rbtree *tree, int key);

void rbtree_purge(struct rbtree *tree);

int rbtree_validate(struct rbtree *tree);

#endif