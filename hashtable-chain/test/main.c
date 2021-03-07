#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "hashmap.h"


#define NL "\n"


struct data {
    int value;
    struct map_node node;
};

static
void shuffle(int32_t *arr, size_t n){
    unsigned int i_excg, tmp;
    srand(9997);
    // for test usage, this produces same psuedo random every time.
    for (size_t i = 0; i < n; ++i){
        i_excg = rand();
        i_excg %= n;
        
        tmp = arr[i];
        arr[i] = arr[i_excg];
        arr[i_excg] = tmp;
    }
    return;
}

static uint32_t data_hasher(struct map_node* mnode){
    struct data *d;
    d = container_of(mnode, struct data, node);
    return d->value;
}

static int data_cmp(struct map_node *a, struct map_node *b){
    struct data *da, *db;
    da = container_of(a, struct data, node);
    db = container_of(b, struct data, node);
    return (da->value == db->value);
}

static int basic_test(void){
    struct data *data_arr, hint, *res;
    struct map *datamap;
    struct map_node *tmp_node;
    const size_t n_data = 197;
    int32_t *int_arr;

    datamap = map_alloc(256, data_hasher, data_cmp);

    int_arr = malloc(sizeof(int32_t) * n_data);
    for (unsigned int i = 0; i < n_data; ++i){
        int_arr[i] = i;
    }
    shuffle(int_arr, n_data);

    data_arr = calloc(n_data, sizeof(struct data));
    for (unsigned int i = 0; i < n_data; ++i){
        data_arr[i].value = int_arr[i];
    }
    memset(&hint, 0, sizeof(struct data));

    printf("test insert"NL);
    for (unsigned int i = 0; i < n_data; ++i){
        map_insert(datamap, &data_arr[i].node);
    }

    printf("test get"NL);
    for (unsigned int i = 0; i < n_data; ++i){
        hint.value = int_arr[i];
        tmp_node = map_get(datamap, &hint.node);
        assert(tmp_node != NULL);
        res = container_of(tmp_node, struct data, node);
        assert(res->value == int_arr[i]);
        //printf("res: %d"NL, res->value);
    }

    printf("test delete"NL);
    for (unsigned int i = 0; i < n_data; ++i){
        map_delete(datamap, &data_arr[i].node);
    }

    printf("test done"NL);

    free(data_arr);
    free(int_arr);
    map_free(datamap);

    printf("free completed"NL);
    
    return 0;
}

int main(void){
    setbuf(stdout, NULL);
    basic_test();
    return 0;
}
