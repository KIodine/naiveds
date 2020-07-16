#ifndef FIFOQ_H
#define FIFOQ_H

#include <stdlib.h>

/* This implementation let indexes overflow freely, the size of
   queue is round up to power of 2 automatically. */

struct fifoq {
    int          *arr;
    unsigned int  size;
    unsigned int  mask;
    unsigned int  front, end;
};
/*
...|X|...
    ^(front, end) -> empty

...| |?|...
    ^ ^(end)
    |(front) -> full
*/

struct fifoq *fifoq_alloc(unsigned int size);
void fifoq_free(struct fifoq *q);

/* Put value in front. */
int fifoq_push(struct fifoq *q, int val);
/* Get value from back. */
int fifoq_get(struct fifoq *q, int *res);
/* Put value in back. */
int fifoq_put(struct fifoq *q, int val);
/* Get value from front. */
int fifoq_pop(struct fifoq *q, int *res);


static int fifoq_is_empty(struct fifoq *q){
    return ((q->front&q->mask) == (q->end&q->mask));
}

static int fifoq_is_full(struct fifoq *q){
    return (((q->front + 1)&q->mask) == (q->end&q->mask));
}

#endif /* FIFOQ_H */