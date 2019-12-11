#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include "bstree.h"


#define NL "\n"


static
void node_traversal(struct bstnode *node){
    if (node == NULL){
        return;
    }
    node_traversal(node->left);
    printf("%d"NL, node->key);
    node_traversal(node->right);
}


void basic_test(void){
    BSTree *tree;
    int res, ret_code;

    printf("alloc"NL);
    tree = bstree_alloc();

    printf("set"NL);
    bstree_set(tree, 123, 456);
    bstree_set(tree, 124, 455);
    bstree_set(tree, 122, 454);

    printf("get"NL);
    bstree_get(tree, 123, &res);
    assert(res == 456);
    bstree_get(tree, 122, &res);
    assert(res == 454);
    ret_code = bstree_get(tree, 0, &res);
    assert(ret_code == BST_NOELEM);

    printf("delete"NL);
    ret_code = bstree_delete(tree, 122);
    assert(ret_code == BST_OK);
    ret_code = bstree_delete(tree, 122);
    assert(ret_code == BST_NOELEM);
    ret_code = bstree_delete(tree, 123);
    assert(ret_code == BST_OK);

    printf("free"NL);
    bstree_free(tree);
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
    
    BSTree *tree = bstree_alloc();

    printf("torture set"NL);
    for (int i = 0; i < count; ++i){
        bstree_set(tree, buffer[i], 0);
    }

    printf("torture delete"NL);
    for (int i = 0; i < 8000; ++i){
        bstree_delete(tree, buffer[1337+i]);
    }

    printf("torture free"NL);
    bstree_free(tree);
    free(buffer);

    return;
}

int main(void){
    basic_test();
    torture_test();
    return 0;
}