#ifndef BINHEAP_H
#define BINHEAP_H

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

void maxheapify(int *arr, int len);

#endif