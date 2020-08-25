#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include "rbtree.h"


#define NL "\n"


static
void node_traversal(struct rbtnode *node){
    if (node == NULL){
        return;
    }
    node_traversal(node->left);
    printf("%d"NL, node->key);
    node_traversal(node->right);
}


void basic_test(void){
    struct rbtree *tree;
    int res, ret_code;

    printf("alloc"NL);
    tree = rbtree_alloc();

    printf("set"NL);
    rbtree_set(tree, 123, 456);
    rbtree_validate(tree);
    rbtree_set(tree, 124, 455);
    rbtree_set(tree, 122, 454);


    printf("get"NL);
    rbtree_get(tree, 123, &res);
    assert(res == 456);
    rbtree_get(tree, 122, &res);
    assert(res == 454);
    ret_code = rbtree_get(tree, 0, &res);
    assert(ret_code == RBT_NOELEM);

    printf("delete"NL);
    ret_code = rbtree_delete(tree, 122);
    assert(ret_code == RBT_OK);
    ret_code = rbtree_delete(tree, 122);
    assert(ret_code == RBT_NOELEM);
    ret_code = rbtree_delete(tree, 123);
    assert(ret_code == RBT_OK);

    rbtree_validate(tree);

    rbtree_purge(tree);
    rbtree_set(tree, 165, 36213);


    printf("free"NL);
    rbtree_free(tree);
    return;
}

void torture_test(void){
    /* insert 65536 ints and delete some of them, finally free */
    unsigned int const count = 65536;
    uint32_t *buffer;
    buffer = malloc(sizeof(uint32_t)*count);
    int fd = open("./testints", O_RDONLY);
    read(fd, buffer, sizeof(uint32_t)*count);
    close(fd);
    
    struct rbtree *tree = rbtree_alloc();

    printf("torture set"NL);
    for (int i = 0; i < count; ++i){
        rbtree_set(tree, buffer[i], 0);
    }
    rbtree_validate(tree);

    printf("torture delete"NL);
    for (int i = 0; i < 8000; ++i){
        rbtree_delete(tree, buffer[1337+i]);
    }
    rbtree_validate(tree);

    printf("torture purge"NL);
    rbtree_purge(tree);
    
    printf("torture set2"NL);
    for (int i = 0; i < count; ++i){
        rbtree_set(tree, buffer[i], 0);
    }
    rbtree_validate(tree);

    printf("torture free"NL);
    rbtree_free(tree);
    free(buffer);

    return;
}

int main(void){
    basic_test();
    torture_test();
    return 0;
}