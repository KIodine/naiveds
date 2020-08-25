#ifndef BINHEAP_H
#define BINHEAP_H

#include <stddef.h>

/* A max heap/priority queue implementation. */

/*
    struct binheap
    
    binheap_alloc()
    binheap_free()
    
    binheap_insert()
    binheap_extract()
    binheap_chgprior()
    maxheapify()
*/

struct binheap {
    int *arr;
    int count, capacity;
};

struct binheap *binheap_alloc(size_t cap);
void binheap_free(struct binheap *heap);

int binheap_insert(struct binheap *heap, int val);
int binheap_extract(struct binheap *heap, int *res);

void maxheapify(int *arr, int len);

#endif