#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "binheap.h"

#define ARRSZ(array) ((sizeof(array))/sizeof(array[0]))

#define RESET "\e[0m"
#define REDTEXT "\e[31m"

#define HEAPSZ 8192


int arr[] = {
    1, 2, 3, 4, 5, 6, 7, 8
};

/* --- util routines --- */
static
void verify_maxheap(int *arr, int len){
    int cur = (len - 1) >> 1, parent;
    
    if (len == 0){
        return;
    }

    for (; cur != 0;){
        parent = ((cur + 1) >> 1) - 1;
        if (arr[cur] > arr[parent]){
            printf(
                REDTEXT"[%d]=%d < [%d]=%d?\n"RESET,
                cur, arr[cur], parent, arr[parent]
            );
            assert(arr[cur] < arr[parent]);
        }
        //assert(cur > parent);
        cur = cur - 1;//parent;
    }
    
    return;
}

/* --- test routines --- */
static
void test_maxheapify(void){
    int sz = ARRSZ(arr);
    
    maxheapify(arr, sz);
    for (int i = 0; i < sz; ++i){
        printf("%d ", arr[i]);
    }
    putchar('\n');
    verify_maxheap(arr, sz);

    return;
}

static
void test_binheap(void){
    struct binheap *heap;
    int sz = HEAPSZ, res;

    heap = binheap_alloc(sz);
    for (int i = 0; binheap_insert(heap, i) == 0; i++){
        ;
    }
    assert(heap->count == sz);
    verify_maxheap(heap->arr, heap->count);

    for (;binheap_extract(heap, &res) == 0;){
        //printf("%d ", res);
        verify_maxheap(heap->arr, heap->count);
    }
    putchar('\n');

    binheap_free(heap);

    return;
}

int main(void){
    printf("--- Test maxheapify ---\n");
    test_maxheapify();
    printf("--- test binheap ---\n");
    test_binheap();
    return 0;
}
