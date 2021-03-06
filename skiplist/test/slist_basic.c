#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "skiplist.h"

static int shuffle_int(int arr[], int n);

static int basic_test(void);
static int main_test(int test_nodes);

//static int const test_nodes = 2600000;
static int const test_scales[] = {
    /*
    16,
    */
    100000,
    200000,
    400000,
    800000,
    1600000,
    /*
    */
};

int main(int argc, char **argv){
    srand(1997);
    //setbuf(stdout, NULL);
    // ---
    basic_test();
    printf("--- Basic test done ---\n");
    for (unsigned int i = 0; i < sizeof(test_scales)/sizeof(test_scales[0]); i++){
        printf("> Testing on %d nodes.\n", test_scales[i]);
        main_test(test_scales[i]);
    }
    printf("--- Main test done ---\n");
    return 0;
}

static int shuffle_int(int arr[], int n){
    for (int i = 0; i < n; i++){
        int tmp, idx = rand()%n;
        tmp = arr[i];
        arr[i] = arr[idx];
        arr[idx] = tmp;
    }
    return 0;
}

static int basic_test(void){
    int ret = 0, res = 0;
    struct skiplist *slist = NULL;

    slist = skiplist_alloc();

    ret = skiplist_get(slist, -1, &res);
    assert(ret == -1);

    ret = skiplist_set(slist, 0, -1);
    assert(ret == 0);
    ret = skiplist_set(slist, 0, -2);
    assert(ret == -1);

    ret = skiplist_get(slist, 0, &res);
    assert(ret == 0);
    assert(res == -1);

    ret = skiplist_del(slist, 0);
    assert(ret == 0);
    ret = skiplist_del(slist, 0);
    assert(ret == -1);

    skiplist_free(slist);
    slist = NULL;

    return 0;
}

static int main_test(int test_nodes){
    int ret, res, *arr;
    struct timespec start, dt;
    long long steps;
    double ns;
    struct skiplist *slist = NULL;

    arr = malloc(test_nodes*sizeof(int));
    for (int i = 0; i < test_nodes; i++){
        arr[i] = i;
    }
    shuffle_int(arr, test_nodes);

    slist = skiplist_alloc();

    /* Setup nodes. */
    printf("Setup nodes...");
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < test_nodes; ++i){
        ret = skiplist_set(slist, arr[i], i);
        assert(ret == 0);
    }
    clock_gettime(CLOCK_MONOTONIC, &dt);
    if (dt.tv_nsec < start.tv_nsec){
        dt.tv_sec -= 1; dt.tv_nsec += 1000000000;
    }
    dt.tv_nsec -= start.tv_nsec;
    dt.tv_sec  -= start.tv_sec;
    ns = (double)dt.tv_nsec + ((double)dt.tv_sec*1e9);
    ns /= (double)test_nodes;
    printf(" %.3f ns/op\n", ns);

    /* Ensure all nodes are inserted and properly set. */
    slist->search_steps = 0;
    printf("fetching nodes...");
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < test_nodes; ++i){
        ret = skiplist_get(slist, arr[i], &res);
        assert(ret == 0);
        //assert(res == i*2);
    }
    clock_gettime(CLOCK_MONOTONIC, &dt);
    if (dt.tv_nsec < start.tv_nsec){
        dt.tv_sec -= 1; dt.tv_nsec += 1000000000;
    }
    dt.tv_nsec -= start.tv_nsec;
    dt.tv_sec  -= start.tv_sec;
    ns = (double)dt.tv_nsec + ((double)dt.tv_sec*1e9);
    ns /= (double)test_nodes;
    printf(" %.3f ns/op\n", ns);
    steps = slist->search_steps/test_nodes;
    
    /* Delete some nodes. */
    printf("delete 75%% nodes...");
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = test_nodes/4; i < test_nodes; ++i){
        ret = skiplist_del(slist, arr[i]);
        assert(ret == 0);
    }
    clock_gettime(CLOCK_MONOTONIC, &dt);
    if (dt.tv_nsec < start.tv_nsec){
        dt.tv_sec -= 1; dt.tv_nsec += 1000000000;
    }
    dt.tv_nsec -= start.tv_nsec;
    dt.tv_sec  -= start.tv_sec;
    ns = (double)dt.tv_nsec + ((double)dt.tv_sec*1e9);
    ns /= (double)(test_nodes-test_nodes/4);
    printf(" %.3f ns/op\n", ns);
    
    /* Ensure those deleted are unreachable. */
    printf("Ensure nodes are deleted...");
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = test_nodes/4; i < test_nodes; ++i){
        ret = skiplist_get(slist, arr[i], &res);
        assert(ret == -1);
    }
    clock_gettime(CLOCK_MONOTONIC, &dt);
    if (dt.tv_nsec < start.tv_nsec){
        dt.tv_sec -= 1; dt.tv_nsec += 1000000000;
    }
    dt.tv_nsec -= start.tv_nsec;
    dt.tv_sec  -= start.tv_sec;
    ns = (double)dt.tv_nsec + ((double)dt.tv_sec*1e9);
    ns /= (double)test_nodes;
    printf(" %.3f ns/op\n", ns);

    printf("Dist of levels:\n");
    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++){
        printf("[%2d]=%d\n", i, slist->dist[i]);
    }
    printf("Avg search steps: %lld\n", steps);

    skiplist_free(slist);
    slist = NULL;
    
    free(arr);
    arr = NULL;

    return 0;
}