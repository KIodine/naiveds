#include <stdio.h>
#include <assert.h>

#include "fifoq.h"


#define NL "\n"
#define ARRSIZE(arr) sizeof(arr)/sizeof(arr[0])


int test_data[] = {
    1, 2, 3, 4, 5
};


void test_fifoq(void){
    int ret;
    struct fifoq *q;

    q = fifoq_alloc(17);
    assert(fifoq_is_empty(q));

    printf("--- test push ---"NL);
    for (int i = 0; !fifoq_push(q, i); ++i){
        ;
    }
    assert(fifoq_is_full(q));

    printf("--- test get ---"NL);
    for (int i = 0; !fifoq_get(q, &ret); ++i){
        ;
    }
    assert(fifoq_is_empty(q));

    printf("--- test put ---"NL);
    for (int i = 0; !fifoq_put(q, i); ++i){
        ;
    }
    assert(fifoq_is_full(q));

    printf("--- test pop ---"NL);
    for (int i = 0; !fifoq_pop(q, &ret); ++i){
        ;
    }
    assert(fifoq_is_empty(q));

    fifoq_free(q);

    return;
}

int main(void){
    test_fifoq();
    return EXIT_SUCCESS;
}