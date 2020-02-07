#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

#include "avltree.h"

#define NL "\n"
#define NINT32 4096


void trav(struct avlnode *node, int depth){
    if (node == NULL) return;
    trav(node->left, depth + 1);
    for (int i = 0; i < depth; ++i){
        printf("  ");
    }
    printf("%d <%d>"NL, node->key, node->height);
    trav(node->right, depth + 1);
    return;
}

void basic_test(void){
    int res, tmp;
    struct avltree *tree;
    
    tree = avl_alloc();

    printf("basic insert test..."NL);
    //trav(tree->root, 0);
    //printf("--------"NL);
    for (int i = 0; i < 10; ++i){
        res = avl_insert(tree, i, 0);
        //trav(tree->root, 0);
        //printf("--------"NL);
        assert(res == 0);
    }
    avl_validate(tree);
    //trav(tree->root, 0);
    printf("yes"NL);


    printf("basic get test..."NL);
    for (int i = 0; i < 10; ++i){
        res = avl_get(tree, i, &tmp);
        assert(res == 0);
    }
    avl_validate(tree);
    printf("yes"NL);


    printf("basic delete test..."NL);
    for (int i = 3; i < 8; ++i){
        res = avl_delete(tree, i);
        assert(res == 0);
    }
    avl_validate(tree);
    printf("yes"NL);

    avl_free(tree);
    tree = NULL;
    return;
}

void torture_test(void){
    return;
}

int main(void){
    printf("start test"NL);
    basic_test();
    //torture_test();
    return 0;
}