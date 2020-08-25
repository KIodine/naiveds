#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "binheap.h"

#define ARRSZ(array) ((sizeof(array))/sizeof(array[0]))


int arr[] = {
    1, 2, 3, 4, 5, 6, 7, 8
};

static
void verify_maxheap(int *arr, int len){
    int cur = (len - 1) >> 1, parent;
    
    for (; cur != 0;){
        parent = ((cur + 1) >> 1) - 1;
        assert(arr[cur] < arr[parent]);
        assert(cur > parent);
        cur = parent;
    }
    
    return;
}

int main(void){
    int sz = ARRSZ(arr);
    
    maxheapify(arr, sz);
    for (int i = 0; i < sz; ++i){
        printf("%d ", arr[i]);
    }
    putchar('\n');
    verify_maxheap(arr, sz);

    return 0;
}
