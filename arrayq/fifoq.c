#include "fifoq.h"

#include <assert.h>


static unsigned int round_up_pow_2(unsigned int v){
    /*
     *  fill bits lower than the leftmost bit.
     *  the decrement and increment prevents numbers already are
     *  power or 2 doubles.
     */
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

struct fifoq *fifoq_alloc(unsigned int size){
    struct fifoq *q;
    
    size = round_up_pow_2(size);
    q = calloc(1, sizeof(struct fifoq));
    
    q->arr   = malloc(sizeof(int)*(size + 1));
    /* extra 1 space for sentinel. */
    q->size  = size;
    q->mask  = size - 1;
    /* Ensure it's power of 2. */
    assert(((size - 1) & size) == 0);
    
    q->front = 0;
    q->end   = 0;
    
    return q;
}

void fifoq_free(struct fifoq *q){
    free(q->arr);
    free(q);
    return;
}

int fifoq_push(struct fifoq *q, int val){
    if (fifoq_is_full(q)){
        return -1;
    }
    q->arr[q->front & q->mask] = val;
    q->front++;
    
    return 0;
}

int fifoq_get(struct fifoq *q, int *res){
    if (fifoq_is_empty(q)){
        q->front = q->end = 0;
        return -1;
    }
    *res = q->arr[q->end & q->mask];
    q->end++;
    
    return 0;
}

int fifoq_put(struct fifoq *q, int val){
    if (fifoq_is_full(q)){
        return -1;
    }
    q->end--;
    q->arr[q->end & q->mask] = val;
    
    return 0;
}

int fifoq_pop(struct fifoq *q, int *res){
    if (fifoq_is_empty(q)){
        return -1;
    }
    q->front--;
    *res = q->arr[q->front & q->mask];

    return 0;
}