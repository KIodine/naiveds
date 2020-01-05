#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

#include "list.h"


#define NL "\n"
#define NINT32 1000000


struct testnode {
    int val;
    struct list node;
};


void basic_test(void){
    int fd;
    uint32_t *arr;
    uint32_t readsz = sizeof(uint32_t)*NINT32;
    struct testnode *nodearr;
    list_decl(head);
    
    fd = open("./testints.bin", O_RDONLY);
    arr = malloc(readsz);
    
    nodearr = malloc(sizeof(struct testnode)*NINT32);
    for (int i = 0; i < NINT32; ++i){
        nodearr[i].val = arr[i];
    }
    read(fd, arr, readsz);
    close(fd);

    for (int i = 0; i < NINT32; ++i){
        list_push(&head, &nodearr[i].node);
    }
    for (int i = 0; i < NINT32; ++i){
        assert(list_pop(&head) != NULL);
    }
    assert(list_is_empty(&head));
    for (int i = 0; i < NINT32; ++i){
        list_append(&head, &nodearr[i].node);
    }
    for (int i = 0; i < NINT32; ++i){
        assert(list_get(&head) != NULL);
    }
    assert(list_is_empty(&head));

    for (int i = 0; i < NINT32; ++i){
        list_push(&head, &nodearr[i].node);
    }
    for (int i = 0; i <NINT32; ++i){
        list_delete(&nodearr[i].node);
    }
    assert(list_is_empty(&head));

    free(nodearr);
    free(arr);
    return;
}

int main(void){
    return 0;
}