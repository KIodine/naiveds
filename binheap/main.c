#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include "binheap.h"


#define NL "\n"
#define N_INT32 1048576

char *testfile = "./testint.bin";



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

void torture_test(void){
    int fd, ret, res;
    uint32_t *arr;
    struct maxheap *heap;

    fd   = open(testfile, O_RDONLY);
    arr  = malloc(N_INT32*sizeof(uint32_t));
    heap = maxheap_alloc();

    read(fd, arr, N_INT32*sizeof(uint32_t));

    puts("torture insert");
    for (int i = 0; i < N_INT32; ++i){
        //putchar('0');
        maxheap_insert(heap, arr[i]);
    }

    puts("torture extract");
    for (;;){
        ret = maxheap_extract(heap, &res);
        if (ret == MAXHEAP_NOELEM){
            break;
        }
    }
    maxheap_purge(heap);
    maxheap_free(heap);
    free(arr);
    return;
}

int main(void){
    basic_test();
    torture_test();
    return 0;
}