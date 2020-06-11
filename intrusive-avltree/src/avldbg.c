#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "avldbg.h"


#define NL "\n"


static void node_print(struct avlnode *node, int depth);
static void node_validate(struct avlnode *node, avl_cmp_t cmp);


void avl_print(struct avltree *tree){
    putchar('\n');
    node_print(tree->root, 0);
    return;
}

static
void node_print(struct avlnode *node, int depth){
    if (node == NULL){
        return;
    }
    node_print(node->child[CLD_R], depth + 1);
    for (int i = 0; i < depth; ++i){
        printf("    ");
    }
    printf("+-[%d]"NL, node->factor);
    node_print(node->child[CLD_L], depth + 1);
    return;
}

void avl_validate(struct avltree *tree){
    node_validate(tree->root, tree->cmp);
    return;
}

static
void node_validate(struct avlnode *node, avl_cmp_t cmp){
    int res;
    if (node == NULL){
        return;
    }
    // AVL basic property
    assert(abs(node->factor) < 2);
    // BST basic property
    if (node->child[CLD_L] && node->child[CLD_R]){
        res = cmp(node->child[CLD_L], node->child[CLD_R]);
        assert(res < 0);
    }
    // more strict basic attribute test
    if (!node->child[CLD_L] && !node->child[CLD_R]){
        assert(node->factor == 0);
    }
    if (node->child[CLD_L] != NULL){
        assert(node->child[CLD_L]->parent == node);
    }
    if (node->child[CLD_R] != NULL){
        assert(node->child[CLD_R]->parent == node);
    }
    node_validate(node->child[CLD_L], cmp);
    node_validate(node->child[CLD_R], cmp);
    return;
}
