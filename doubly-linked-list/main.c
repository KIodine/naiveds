#include <stdio.h>
#include <assert.h>

#include "dllist.h"

#define NL "\n"


void basic_test(void){
    // alloc
    int res;
    struct dllist *list;

    printf("alloc"NL);
    list = dll_alloc();
    // push and pop

    printf("push pop"NL);
    for (int i = 0; i < 8; ++i){
        dll_push(list, i);
    }
    for (int i = 0; i < 8; ++i){
        assert(dll_pop(list, &res) == DLL_OK);
    }
    assert(dll_pop(list, &res) == DLL_NOELEM);

    // append and get
    printf("append get"NL);
    for (int i = 0; i < 8; ++i){
        dll_append(list, i);
    }
    for (int i = 0; i < 8; ++i){
        assert(dll_get(list, &res) == DLL_OK);
    }
    assert(dll_get(list, &res) == DLL_NOELEM);

    // push and get
    printf("push get"NL);
    for (int i = 0; i < 8; ++i){
        dll_push(list, i);
    }
    for (int i = 0; i < 8; ++i){
        assert(dll_get(list, &res) == DLL_OK);
    }
    assert(dll_get(list, &res) == DLL_NOELEM);
    
    // push and delete
    printf("push delete"NL);
    for (int i = 0; i < 8; ++i){
        dll_push(list, i);
    }
    assert(dll_delete(list, 0) == DLL_OK);
    assert(dll_delete(list, 0) == DLL_NOELEM);

    // purge and free
    printf("purge free"NL);
    dll_purge(list);
    dll_free(list);
    return;
}

int main(void){
    basic_test();
    return 0;
}