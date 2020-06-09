#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include "avltree.h"

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
    // for test usage, this produces same psuedo random every time.
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
void basic_test(void){
    avl_tree_decl(tree, pair_cmp);
    int const N_NODES = 1000000;
    int ret, *narr;
    struct pair *parr, *tmp_pair, hint_pair;
    struct avlnode *tmp_node;
    
    memset(&hint_pair.node, 0, sizeof(struct avlnode));
    parr = calloc(N_NODES, sizeof(struct pair));
    
    narr = malloc(N_NODES*sizeof(int));
    for (int i = 0; i < N_NODES; ++i){
        narr[i] = i;
    }
    shuffle(narr, N_NODES);

    printf("basic insert...");
    for (int i = 0; i < N_NODES; ++i){
        parr[i].key = narr[i];
        tmp_node = avl_insert(&tree, &parr[i].node);
        assert(tmp_node == &parr[i].node);
    }
    avl_validate(&tree);
    printf("ok"NL);

    printf("basic get...");
    for (int i = 0; i < N_NODES; ++i){
        hint_pair.key = narr[i];
        tmp_node = avl_get(&tree, &hint_pair.node);
        assert(tmp_node != NULL);
        tmp_pair = container_of(tmp_node, struct pair, node);
        assert(tmp_pair->key == narr[i]);
    }
    printf("ok"NL);

    printf("basic delete...");
    for (int i = 500; i < 6000; ++i){
        tmp_pair = &parr[i];
        tmp_node = avl_delete(&tree, &tmp_pair->node);
        assert(tmp_node == &tmp_pair->node);
    }
    avl_validate(&tree);
    printf("ok"NL);

    printf("basic test done"NL);
    free(parr);
    free(narr);
    return;
}

static
void torture_test(void){
    avl_tree_decl(tree, pair_cmp);
    int ret;
    int32_t *narr;
    struct pair *parr, *tmp_pair, hint_pair;
    struct avlnode *tmp_node;

    parr = calloc(NNUMS, sizeof(struct pair));
    narr = malloc(sizeof(int32_t)*NNUMS);
    
    for (int i = 0; i < NNUMS; ++i){
        narr[i] = i;
    }
    shuffle(narr, NNUMS);

    printf("torture insert...");
    for (int i = 0; i < NNUMS; ++i){
        parr[i].key = narr[i];
        tmp_node = avl_insert(&tree, &parr[i].node);
        //assert(ret == 0);
    }
    avl_validate(&tree);
    printf("ok"NL);

    printf("torture get...");
    for (int i = 0; i < NNUMS; ++i){
        hint_pair.key = narr[i];
        tmp_node = avl_get(&tree, &hint_pair.node);
        assert(tmp_node != NULL);
    }
    printf("ok"NL);

    printf("torture delete...");
    for (int i = 25; i < 3100; ++i){
        tmp_pair = &parr[i];
        tmp_node = avl_delete(&tree, &tmp_pair->node);
        assert(tmp_node == &tmp_pair->node);
    }
    avl_validate(&tree);
    printf("ok"NL);

    free(narr);
    free(parr);

    return;
}

int main(void){
    // forces stdout flush
    setbuf(stdout, NULL);
    
    basic_test();
    torture_test();
    return 0;
}