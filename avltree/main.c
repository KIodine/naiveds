#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

#include "avltree.h"

#define NL "\n"
#define NINT32 4096


void trav(struct avlnode *node, int depth){
    if (node == NULL){
        return;
    }
    trav(node->child[CLD_L], depth + 1);
    for (int i = 0; i < depth; ++i){
        printf("  ");
    }
    printf("%d <%d>"NL, node->key, node->height);
    trav(node->child[CLD_R], depth + 1);
    return;
}

void basic_test(void){
    int res, tmp;
    struct avltree *tree;
    
    tree = avl_alloc();

    printf("basic insert test..."NL);
    for (int i = 0; i < 64; ++i){
        res = avl_insert(tree, i, 0);
        assert(res == 0);
    }
    avl_validate(tree);
    printf("yes"NL);


    printf("basic get test..."NL);
    for (int i = 0; i < 64; ++i){
        res = avl_get(tree, i, &tmp);
        assert(res == 0);
    }
    avl_validate(tree);
    printf("yes"NL);


    printf("basic delete test..."NL);
    for (int i = 30; i < 62; ++i){
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
    int fd, res, qres;
    uint32_t *elems;
    struct avltree *tree;
    
    tree = avl_alloc();
    elems = malloc(sizeof(uint32_t)*NINT32);
/*
    for (int i = 0; i < NINT32; ++i){
        elems[i] = (uint32_t)i;
    }
*/
    fd = open("./test.bin", O_RDONLY);
    read(fd, elems, sizeof(uint32_t)*NINT32);
    close(fd);

    printf("torture insert"NL);
    for (int i = 0; i < NINT32; ++i){
        res = avl_insert(tree, elems[i], 0);
        assert(res == 0);
    }
    avl_validate(tree);
    printf("done"NL);

    printf("torture get"NL);
    for (int i = 0; i < NINT32; ++i){
        res = avl_get(tree, elems[i], &qres);
        assert(res == 0);
    }
    printf("done"NL);

    printf("torture delete"NL);
    for (int i = 0; i < 1000; ++i){
        res = avl_delete(tree, elems[i]);
        //printf("%d: %16d"NL, i, elems[i]);
    }
    avl_validate(tree);
    printf("done"NL);

    avl_free(tree);
    tree = NULL;

    free(elems);

    return;
}

int main(void){
    printf("start test"NL);
    basic_test();
    torture_test();
    return 0;
}