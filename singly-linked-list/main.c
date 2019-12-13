#include <stdio.h>
#include <assert.h>

#include "sllist.h"

#define NL "\n"

void basic_test(void){
    int ret_code, res;
    struct sllist *list;
    list = sll_alloc();

    printf("test push"NL);
    for (int i = 0; i < 8; ++i){
        ret_code = sll_push(list, i);
        assert(ret_code == SLL_OK);
    }
    LIST_TRAVERSE(list, node){
        //printf("%d ", node->val);
    }
    //putchar('\n');
    
    printf("test pop"NL);
    for (int i = 7; i >= 0; --i){
        ret_code = sll_pop(list, &res);
        assert(ret_code == SLL_OK);
        //printf("%d " , res);
        assert(res == i);
    }
    //putchar('\n');

    printf("test empty pop"NL);
    ret_code = sll_pop(list, &res);
    assert(ret_code == SLL_NOELEM);

    printf("test append"NL);
    for (int i = 0; i < 8; ++i){
        ret_code = sll_append(list, i);
        assert(ret_code == SLL_OK);
    }
    int i = 0;
    LIST_TRAVERSE(list, n){
        assert(n->val == i);
        ++i;
    }
    printf("test get"NL);
    ret_code = sll_get(list, 3U, &res);
    assert(ret_code == SLL_OK);
    assert(res == 3);

    printf("test delete"NL);
    ret_code = sll_delete(list, 3);
    assert(ret_code == SLL_OK);

    printf("test empty delete"NL);
    ret_code = sll_delete(list, 3);
    assert(ret_code == SLL_NOELEM);

    printf("test reverse"NL);
    sll_reverse(list);
    
    printf("check reverse"NL);
    LIST_TRAVERSE(list, node){
        printf("%d"NL, node->val);
    }

    printf("test purge"NL);
    sll_purge(list);

    printf("test free"NL);
    sll_free(list);
    return;
}

int main(void){
    basic_test();
    return 0;
}