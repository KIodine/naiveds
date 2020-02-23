#include <stdio.h>
#include <stdlib.h>

#include "avltree.h"

#define NL "\n"


struct pair {
    struct avlnode node;
    int key;
};

int pair_cmp(struct avlnode const *a, struct avlnode const *b){
    struct pair *pa, *pb;
    int cmpres;
    pa = container_of(a, struct pair, node);
    pb = container_of(b, struct pair, node);
    if (pa->key > pb->key){
        cmpres = NODE_GT;
    } else if (pa->key < pb->key){
        cmpres = NODE_LT;
    } else {
        cmpres = NODE_EQ;
    }
    return cmpres;
}

void basic_test(void){
    avl_tree_decl(tree, pair_cmp);
    int ret;
    struct pair *parr, *tmp_pair, hint_pair;
    struct avlnode *tmp_node;
    
    memset(&hint_pair.node, 0, sizeof(struct avlnode));
    parr = calloc(10, sizeof(struct pair));
    
    printf("basic insert...");
    for (int i = 0; i < 10; ++i){
        parr[i].key = i;
        ret = avl_insert(&tree, &parr[i].node);
        assert(ret == 0);
        //avl_print(&tree);
    }
    avl_print(&tree);
    avl_validate(&tree);
    printf("ok"NL);

    printf("basic get...");
    for (int i = 0; i < 10; ++i){
        hint_pair.key = i;
        tmp_node = avl_get(&tree, &hint_pair.node);
        assert(tmp_node != NULL);
        tmp_pair = container_of(tmp_node, struct pair, node);
        assert(tmp_pair->key == i);
    }
    printf("ok"NL);

    printf("basic delete...");
    for (int i = 2; i < 8; ++i){
        tmp_pair = &parr[i];
        ret = avl_delete(&tree, &tmp_pair->node);
        assert(ret == 0);
        avl_print(&tree);
        printf("------------"NL);
    }
    avl_print(&tree);
    avl_validate(&tree);
    printf("ok"NL);

    printf("basic test done"NL);
    free(parr);
    return;
}

void torture_test(void){
    return;
}

int main(int argc, char *argv[]){
    // forces stdout flush
    setbuf(stdout, NULL);
    
    basic_test();
    //torture_test();
    return 0;
}