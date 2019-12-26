#include <stdio.h>

#include "binheap.h"


void basic_test(void){
    /*
     * Alloc heap
     * insert random numbers
     * extract until empty
     * insert random numbers
     * purge
     * insert random numbers
     * extract until empty
     * free
     */
    struct maxheap *heap;
    int ret, res;

    heap =  maxheap_alloc();
    for (int i = 0; i < 32; ++i){
        maxheap_insert(heap, i);
    }
    for (;(ret = maxheap_extract(heap, &res)) != MAXHEAP_NOELEM;){
        printf("%d ", res);
    }
    putchar('\n');

    maxheap_purge(heap);
    for (int i = 0; i < 32; ++i){
        maxheap_insert(heap, i);
    }
    for (;(ret = maxheap_extract(heap, &res)) != MAXHEAP_NOELEM;){
        printf("%d ", res);
    }
    putchar('\n');

    maxheap_free(heap);
    return;
}

int main(void){
    basic_test();
    return 0;
}