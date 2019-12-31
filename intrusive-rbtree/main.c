#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include "rbtree.h"


#define NL          "\n"
#define NINT32      65536
#define TESTFNAME   "./testints.bin"


struct pair {
    struct rbtnode node;
    int key, val;
};


int paircmp(struct rbtnode *a, struct rbtnode *b){
    struct pair *pa, *pb;
    int ret;
    pa = container_of(a, struct pair, node);
    pb = container_of(b, struct pair, node);
    if (pa->key > pb->key){
        ret = CMP_GT;
    } else if (pa->key < pb->key){
        ret = CMP_LT;
    } else {
        ret = CMP_EQ;
    }
    return ret;
}

void basic_test(void){
    int fd, *arr;
    struct rbtree tree;
    struct pair sentinel, hint, *pair_arr;
    
    fd = open(TESTFNAME, O_RDONLY);
    arr = malloc(sizeof(uint32_t)*NINT32);
    read(fd, arr, sizeof(uint32_t)*NINT32);
    close(fd);
    
    pair_arr = calloc(NINT32, sizeof(struct pair));
    rbtree_init(&tree, &sentinel.node, paircmp);
    for (int i = 0; i < NINT32; ++i){
        struct pair *tmp = &pair_arr[i];
        tmp->node.parent = NULL;
        tmp->node.left   = NULL;
        tmp->node.right  = NULL;
        tmp->key = i;
        rbtree_set(&tree, &tmp->node);
    }
    rbtree_validate(&tree);

    for (int i = 0; i < NINT32; ++i){
        hint.key = i;
        assert(rbtree_get(&tree, &hint.node) != NULL);
    }
    rbtree_validate(&tree);

    for (int i = 0; i < NINT32; ++i){
        struct pair *tmp = &pair_arr[i];
        rbtree_delete(&tree, &tmp->node);
    }
    rbtree_validate(&tree);

    free(arr);
    free(pair_arr);

    return;
}

void torture_test(void){
    return;
}

int main(void){
    basic_test();
    torture_test();
    return 0;
}