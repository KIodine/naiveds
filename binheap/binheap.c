#include <assert.h>
#include <stdlib.h>


#include "binheap.h"

/*
    get_parent()
    get_left()
    get_right()
    get_last_parent()
    sift_down()
    sift_up()
*/

/* --- static routines --- */
static inline
int get_parent(int cur){
    return ((cur + 1) >> 1) - 1;
}

static inline
int get_left(int cur){
    return ((cur + 1) << 1) - 1;
}

static inline
int get_right(int cur){
    return ((cur + 1) << 1);
}

static inline
int get_last_parent(int len){
    return (len - 1) >> 1;
}

static inline
void sift_up(int *arr, int cur){
    int parent, tmp;

    for (; cur != 0;){
        parent = get_parent(cur);
        if (arr[cur] > arr[parent]){
            // do swap
            tmp         = arr[cur];
            arr[cur]    = arr[parent];
            arr[parent] = tmp;
        } else {
            break;
        }
        /* Ensure the procedure ends eventually. */
        assert(cur > parent);
        cur = parent;
    }

    return;
}

static inline
void sift_down(int *arr, int cur, int len){
    int left, right, subst, tmp;

    for (;cur < len;){
        subst = cur;
        left  = get_left(cur);
        right = left + 1;

        /* boundary check & choose subst. */
        if (left < len){
            subst = left;
        }
        if (right < len && (arr[right] > arr[left])){
            subst = right;
        }
        if (subst == cur){
            /* no available child to subst. */
            break;
        }

        /* try do swap. */
        if (arr[subst] > arr[cur]){
            /* do swap. */
            tmp        = arr[subst];
            arr[subst] = arr[cur];
            arr[cur]   = tmp;
        } else {
            break;
        }

        /* `subst` must be a increasing number. */
        assert(subst > cur);

        cur = subst;
    }

    return;
}


/* --- public routines --- */

void maxheapify(int *arr, int len){
    int cur;
    for (cur = get_last_parent(len); ; cur--){
        sift_down(arr, cur, len);
        if (cur == 0){
            break;
        }
    }
    return;
}

struct binheap *binheap_alloc(size_t cap){
    struct binheap *heap;
    int *arr;

    heap = calloc(1, sizeof(struct binheap));
    if (heap == NULL){
        goto heap_alloc_fail;
    }
    arr = calloc(1, sizeof(int)*cap);
    if (arr == NULL){
        goto arr_alloc_fail;
    }

    heap->arr      = arr;
    heap->count    = 0;
    heap->capacity = cap;

    return heap;
    /* Error handling section. */
arr_alloc_fail:
    free(heap);
heap_alloc_fail:
    return NULL;
}

void binheap_free(struct binheap *heap){
    free(heap->arr);
    free(heap);
    return;
}

int binheap_insert(struct binheap *heap, int val){
    int cur;

    if (heap->count >= heap->capacity){
        return -1;
    }

    cur = heap->count;
    heap->arr[cur] = val;
    heap->count   += 1;
    
    sift_up(heap->arr, cur);

    return 0;
}


int binheap_extract(struct binheap *heap, int *res){
    int cur;

    if (heap->count == 0 || res == NULL){
        return -1;
    }

    cur = heap->count;

    /* Return max value and subst with last value in heap. */
    *res = heap->arr[0];
    heap->arr[0] = heap->arr[cur-1];
    heap->count -= 1;

    sift_down(heap->arr, 0, heap->count);

    return 0;
}

// TODO
