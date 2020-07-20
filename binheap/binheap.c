#include "binheap.h"


static void heap_grow(struct maxheap *heap){
    /* alloc new array, copy, free old array */
    int new_sz, *new_arr;
    new_sz  = heap->capacity << 1; // just simply grow twice big.
    new_arr = calloc(new_sz, sizeof(int));
    
    memcpy(new_arr, heap->arr, heap->capacity*sizeof(int));
    free(heap->arr);
    
    heap->arr      = new_arr;
    heap->capacity = new_sz;
    return;
}


void maxheapify(int *arr, unsigned int len){
    int parent, tmp;
    /* for every node: if node > parent, swap(node, parent) */
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
    int cur, parent, tmp, *arr;
    cur = heap->count++;
    /* TODO: grow if required */
    if (heap->capacity <= heap->count){
        heap_grow(heap);
    }
    arr = heap->arr;
    arr[cur] = val;
    cur++;
    for (;;){
        parent = cur >> 1;
        if (parent == 0){ break;}
        if (arr[parent-1] < arr[cur-1]){
            tmp           = arr[cur-1];
            arr[cur-1]  = arr[parent-1];
            arr[parent-1] = tmp;
            cur = parent;
        } else {
            break;
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
    int cur = 1, child, tmp, *arr;
    
    if (heap->count == 0){
        return MAXHEAP_NOELEM;
    };
    arr = heap->arr;

    /* swap first and last. */
    *res   = arr[0];
    arr[0] = arr[heap->count-1];
    arr[heap->count-1] = *res;
    
    heap->count -= 1;
    
    /* Sift down method. */
    for (;;){
        child = cur << 1;
        if (child >= heap->count){
            /* out-of-range. */
            break;
        }
        assert(child < heap->count);
        
        /* switch if parent is smaller than one of its children */
        if (arr[cur-1] > arr[child-1] && arr[cur-1] > arr[child]){
            /* We've moved the element to the right place */
            break;
        }
        if (arr[child-1] < arr[child]){
            /* if left gt right */
            child += 1;
        }
        /* do swap. */
        tmp          = arr[child-1];
        arr[child-1] = arr[cur-1];
        arr[cur-1] = tmp;
        /* cur move on. */
        cur = child;
    }
    return MAXHEAP_OK;
}
