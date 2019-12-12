#ifndef BSTREE_H
#define BSTREE_H

#include <stdlib.h>


struct bstree {
    unsigned int count;
    struct bstnode *root;
};

struct bstnode {
    int key, val;
    struct bstnode *left, *right;
};


enum {
    BST_OK,
    BST_NOELEM,
};


struct bstree *bstree_alloc(void);
void bstree_free(struct bstree *tree);
int bstree_set(struct bstree *tree, int key, int val);
int bstree_get(struct bstree *tree, int key, int *res);
int bstree_delete(struct bstree *tree, int key);

void bstree_purge(struct bstree *tree);

#endif