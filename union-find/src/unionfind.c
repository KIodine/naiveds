#include <assert.h>
#include "unionfind.h"

#define ALIAS_OF(func) __attribute__((weak, alias(#func)))
#define NO_UNUSED __attribute__((unused))

/* --- static functions --- */
/* NOTE: Implement different find with static function? */
NO_UNUSED
static struct ufnode *find_path_halving(struct ufnode *node){
    struct ufnode *parent;
    for (;node->parent != node;){
        parent = node->parent;
        /* Move parent node upward. */
        node->parent = parent->parent;
        /* Move upward. */
        node = node->parent;
    }
    return node;
}
NO_UNUSED
static struct ufnode *find_path_compress(struct ufnode *node){
    /* Recursive approach of `path-compression`. */
    if (node->parent != node){
        node->parent = uf_find(node->parent);
        return node->parent;
    } else {
        return node;
    }
}

/* --- public functions --- */
void uf_init_set(struct ufnode *node){
    node->parent = node;
    node->rank = 0;
    return;
}

struct ufnode *uf_find(struct ufnode *node) ALIAS_OF(find_path_compress);

//struct ufnode *uf_find(struct ufnode *node){
//    return find_path_compress(node);
//}

int uf_connected(struct ufnode *a, struct ufnode *b){
    a = uf_find(a);
    b = uf_find(b);
    return (a == b);
}

int uf_union(struct ufnode *a, struct ufnode *b){
    struct ufnode *tmp;
    a = uf_find(a);
    b = uf_find(b);

    /* append "smaller" set to the "bigger" one. */
    if (a->rank < b->rank){
        tmp = a;
        a   = b;
        b   = tmp;
    }
    /* a < b: a->parent = b */
    /* a > b: b->parent = a */
    b->parent = a;
    if (a->rank == b->rank){
        a->rank += 1;
    }

    return 0;
}
