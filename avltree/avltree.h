#ifndef AVLTREE_H
#define AVLTREE_H


#include <stdlib.h>


struct avlnode {
    struct avlnode *left, *right;
    int key, val, height;
};

struct avltree {
    struct avlnode *root;
    int count;
};


struct avltree *avl_alloc(void);
void avl_purge(struct avltree *tree);
void avl_free(struct avltree *tree);

int avl_insert(struct avltree *tree, int key, int val);
int avl_get(struct avltree *tree, int key, int *res);
int avl_delete(struct avltree *tree, int key);


#endif /* AVLTREE_H */