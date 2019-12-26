#include "binheap.h"


static void heap_grow(struct maxheap *heap){
    /* alloc new array, copy, free old array */
    return;
}


void maxheapify(int *arr, unsigned int len){
    int parent, tmp;
    for (int i = len; i > 1; --i){
        parent = (i >> 1);
        if (arr[i-1] > arr[parent-1]){
            tmp             = arr[i-1];
            arr[i-1]        = arr[parent-1];
            arr[parent-1]   = tmp;
        }
    }
    return;
}

struct maxheap *maxheap_alloc(void){
    struct maxheap *heap;
    heap = calloc(1, sizeof(struct maxheap));
    heap->capacity = INIT_CAPACITY;
    heap->arr = calloc(INIT_CAPACITY, sizeof(int));
    return heap;
}

void maxheap_free(struct maxheap *heap){
    free(heap->arr);
    free(heap);
    return;
}

void maxheap_purge(struct maxheap *heap){
    free(heap->arr);
    heap->count     = 0;
    heap->capacity  = INIT_CAPACITY;
    heap->arr       = calloc(INIT_CAPACITY, sizeof(int));
    return;
}

int maxheap_insert(struct maxheap *heap, int val){
    int index, parent, tmp, *arr;
    index = heap->count++;
    arr = heap->arr;
    arr[index] = val;
    index++;
    for (;;){
        parent = index >> 1;
        if (parent == 0){ break;}
        if (arr[parent-1] < arr[index-1]){
            tmp           = arr[index-1];
            arr[index-1]  = arr[parent-1];
            arr[parent-1] = tmp;
            index = parent;
        }
    }
    return MAXHEAP_OK;
}

int maxheap_peek(struct maxheap *heap, int *res){
    if (heap->count == 0){ return MAXHEAP_NOELEM;};
    *res = heap->arr[0];
    return MAXHEAP_OK;
}

int maxheap_extract(struct maxheap *heap, int *res){
    int index = 1, child, tmp, *arr;
    if (heap->count == 0){ return MAXHEAP_NOELEM;};
    arr = heap->arr;
    *res = arr[0];
    arr[0] = arr[heap->count-1];
    heap->count -= 1;
    for (;;){
        child = index << 1;
        if (child > heap->count){ break;}
        /* switch if parent is smaller than one of its children */
        if (arr[index-1] > arr[child-1] && arr[index-1] > arr[child]){
            /* We've moved the element to the right place */
            break;
        }
        if (arr[child-1] < arr[child]){
            /* if left gt right */
            child += 1;
        }
        tmp          = arr[child-1];
        arr[child-1] = arr[index-1];
        arr[index-1] = tmp;
        index = child;
    }
    return MAXHEAP_OK;
}
