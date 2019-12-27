#ifndef BINHEAP_H
#define BINHEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define INIT_CAPACITY 32


struct maxheap {
    int count, capacity;
    int *arr;
};

enum {
    MAXHEAP_OK,
    MAXHEAP_NOELEM,
};


void maxheapify(int *arr, unsigned int len);

struct maxheap *maxheap_alloc(void);
void maxheap_free(struct maxheap *heap);
void maxheap_purge(struct maxheap *heap);

int maxheap_insert(struct maxheap *heap, int val);
int maxheap_peek(struct maxheap *heap, int *res);
int maxheap_extract(struct maxheap *heap, int *res);


#endif