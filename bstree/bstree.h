#ifndef BSTREE_H
#define BSTREE_H

#include <stdlib.h>


typedef struct bstree  BSTree;
typedef struct bstnode BSTNode;

struct bstree {
    unsigned int count;
    BSTNode *root;
};

struct bstnode {
    int key, val;
    BSTNode *left, *right;
};


enum {
    BST_OK,
    BST_NOELEM,
};


BSTree *bstree_alloc(void);
void    bstree_free(BSTree *tree);
int bstree_set(BSTree *tree, int key, int val);
int bstree_get(BSTree *tree, int key, int *res);
int bstree_delete(BSTree *tree, int key);


#endif