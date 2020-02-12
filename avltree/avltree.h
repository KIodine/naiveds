#ifndef AVLTREE_H
#define AVLTREE_H


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// TODO: use `struct avlnode *[2]` to store left and right children.
//       by doing so, specific style of coding can be performed.
struct avlnode {
    struct avlnode *child[2];
    int key, val, height;
};

struct avltree {
    struct avlnode *root;
    int count;
};

enum {
    CLD_L = 0,
    CLD_R = 1,
};

struct avltree *avl_alloc(void);
void avl_purge(struct avltree *tree);
void avl_free(struct avltree *tree);

int avl_insert(struct avltree *tree, int key, int val);
int avl_get(struct avltree *tree, int key, int *res);
int avl_delete(struct avltree *tree, int key);

void avl_validate(struct avltree *tree);
// void avl_avlize(struct avltree *tree);

#endif /* AVLTREE_H */