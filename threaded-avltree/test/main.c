#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

#include "avltree.h"
#include "avldbg.h"

#define NL "\n"
#define NNUMS 4096


struct pair {
    struct avlnode node;
    int key;
};

static
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


static
void shuffle(int32_t *arr, size_t n){
    unsigned int i_excg, tmp;
    srand(9997);
    /* for test purpose, this produces same result every time. */
    for (size_t i = 0; i < n; ++i){
        i_excg = rand();
        i_excg %= n;
        
        tmp = arr[i];
        arr[i] = arr[i_excg];
        arr[i_excg] = tmp;
    }
    return;
}

static
void do_test(void){
    //avl_tree_decl(tree, pair_cmp);
    struct avltree tree;
    int const N_NODES = 100000;
    int *narr;
    struct pair *parr, *tmp_pair, hint_pair;
    struct avlnode *tmp_node;
    
    avl_tree_init(&tree, pair_cmp);

    memset(&hint_pair.node, 0, sizeof(struct avlnode));
    parr = calloc(N_NODES, sizeof(struct pair));
    
    narr = malloc(N_NODES*sizeof(int));
    for (int i = 0; i < N_NODES; ++i){
        narr[i] = i;
    }
    shuffle(narr, N_NODES);

    printf("test insert...");
    for (int i = 0; i < N_NODES; ++i){
        parr[i].key = narr[i];
        tmp_node = avl_insert(&tree, &parr[i].node);
        assert(tmp_node == &parr[i].node);
    }
    avl_validate(&tree);
    printf("ok"NL);

    printf("test get...");
    for (int i = 0; i < N_NODES; ++i){
        hint_pair.key = narr[i];
        tmp_node = avl_get(&tree, &hint_pair.node);
        assert(tmp_node != NULL);
        tmp_pair = container_of(tmp_node, struct pair, node);
        assert(tmp_pair->key == narr[i]);
    }
    printf("ok"NL);

    printf("test delete...");
    for (int i = 500; i < 6000; ++i){
        tmp_pair = &parr[i];
        tmp_node = avl_delete(&tree, &tmp_pair->node);
        assert(tmp_node == &tmp_pair->node);
    }
    avl_validate(&tree);
    printf("ok"NL);

    printf("Function test done"NL);
    free(parr);
    free(narr);
    return;
}

int main(void){
    // forces stdout flush
    setbuf(stdout, NULL);
    do_test();
    return 0;
}