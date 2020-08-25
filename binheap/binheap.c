#include <assert.h>

#include "binheap.h"

/*
    get_parent()
    get_left()
    get_right()
    get_last_parent()
    sift_down()
    sift_up()
*/

/* --- static routines --- */
static inline
int get_parent(int cur){
    return ((cur + 1) >> 1) - 1;
}

static inline
int get_left(int cur){
    return ((cur + 1) << 1) - 1;
}

static inline
int get_right(int cur){
    return ((cur + 1) << 1);
}

static inline
int get_last_parent(int len){
    return (len - 1) >> 1;
}

static inline
void sift_up(int *arr, int cur, int len){
    int parent, tmp;

    for (; cur != 0;){
        parent = get_parent(cur);
        if (arr[cur] > arr[parent]){
            // do swap
            tmp         = arr[cur];
            arr[cur]    = arr[parent];
            arr[parent] = tmp;
        } else {
            break;
        }
        /* Ensure the procedure ends eventually. */
        assert(cur < parent);
        cur = parent;
    }

    return;
}

static inline
void sift_down(int *arr, int cur, int len){
    int left, right, subst, tmp;

    for (;cur < len;){
        subst = cur;
        left  = get_left(cur);
        right = left + 1;

        /* boundary check & choose subst. */
        if (left < len){
            subst = left;
        }
        if (right < len && (arr[right] > arr[left])){
            subst = right;
        }
        if (subst == cur){
            /* no available child to subst. */
            break;
        }

        /* try do swap. */
        if (arr[subst] > arr[cur]){
            /* do swap. */
            tmp        = arr[subst];
            arr[subst] = arr[cur];
            arr[cur]   = tmp;
        } else {
            break;
        }

        /* `subst` must be a increasing number. */
        assert(subst > cur);

        cur = subst;
    }

    return;
}


/* --- public routines --- */

void maxheapify(int *arr, int len){
    int cur;
    for (cur = get_last_parent(len); ; cur--){
        sift_down(arr, cur, len);
        if (cur == 0){
            break;
        }
    }
    return;
}
// TODO
