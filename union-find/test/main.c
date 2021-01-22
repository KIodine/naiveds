#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "unionfind.h"

#define NL "\n"

#define ARRAYSZ(arr) (sizeof(arr)/sizeof(arr[0]))

static struct ufnode sets[8192];


static void uf_test(void){
    static const int arrsz = ARRAYSZ(sets);
    printf("[union-find] set size=%d"NL, arrsz);

    for (int i = 0; i < arrsz; ++i){
        uf_init_set(&sets[i]);
        assert(1 == uf_connected(&sets[i], &sets[i]));
    }

    /* Test union with other in set. */
    for (int i = 0; i < (arrsz - 1); i++){
        int un_subj = i + 1;
        uf_union(&sets[i], &sets[un_subj]);
        for (int j = 0; j <= i; j++){
            /* for each set number smaller/eq than that, test 
               connectivity */
            assert(1 == uf_connected(&sets[j], &sets[un_subj]));
        }
        for (int j = un_subj + 1; j < arrsz; j++){
            /* for others must remain disconnected. */
            assert(0 == uf_connected(&sets[un_subj], &sets[j]));
        }
    }

    return;
}

int main(void){
    printf("--- test union-find ---"NL);
    uf_test();
    return 0;
}