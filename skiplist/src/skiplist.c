#include "skiplist.h"
#include "slist_internal.h"


static int generate_level(void);
static struct skiplist_node *slist_node_alloc(int level);
static struct skiplist_node *skiplist_get_ex(
    struct skiplist *slist, int key,
    struct skiplist_node *prevs[SKIPLIST_MAX_LEVEL]
);
//static void slist_node_free(struct skiplist_node *snode);
/* Just use normal `free` to dealloc node. */


static int generate_level(void){
    int level = 0;
    // Branch factor does impact performance?
    for (;level < SKIPLIST_MAX_LEVEL;){
        /* approximately 1/4 ~= 0.25 */
        // or better implementation?
        if (rand() < RAND_MAX/2 && level < (SKIPLIST_MAX_LEVEL-1)){
            level++;
            continue; // damn.
        }
        break;
    }
    assert(level < SKIPLIST_MAX_LEVEL);
    return level;
}

static struct skiplist_node *slist_node_alloc(int level){
    struct skiplist_node *snode = NULL;
    //
    if (level < 0){
        return NULL;
    }

    snode = calloc(
        1, sizeof(struct skiplist_node)+sizeof(struct skiplist_link)*(level+1)
    );
    // links = calloc(1, sizeof(struct skiplist_link)*(level+1));
    // snode->links = links
    if (snode == NULL){
        return NULL;
    }
    snode->level = level;

    return snode;
}


struct skiplist *skiplist_alloc(void){
    struct skiplist *slist = NULL;
    struct skiplist_node *header = NULL;

    slist = calloc(1, sizeof(struct skiplist));
    if (slist == NULL){
        goto slist_fail;
    }
    header = slist_node_alloc(SKIPLIST_MAX_LEVEL);
    if (header == NULL){
        goto header_fail;
    }
    slist->header = header;

    return slist;
header_fail:
    free(slist);
slist_fail:
    return NULL;
}

void skiplist_free(struct skiplist *slist){
    struct skiplist_node *cur = NULL, *nxt = NULL;

    /* The lowest level consists all links. */
    cur = slist->header->links[0].next;
    for (;cur != NULL;){
        nxt = cur->links[0].next;
        free(cur);
        cur = nxt;
    }
    free(slist->header);
    free(slist);

    return;
}

/* This is the fundamental of all skiplist operation, especially its
   argument `pprevs`, records the place a node (could) at. Return value
   indicates whether the node exists or not. */
static struct skiplist_node *skiplist_get_ex(
    struct skiplist *slist, int key,
    struct skiplist_node *prevs[SKIPLIST_MAX_LEVEL]
){
    // ---
    struct skiplist_node *nxt, *cur, *found = NULL;
    //struct skiplist_link *links;
    int cur_lvl = SKIPLIST_MAX_LEVEL-1;//slist->current_max_level;

    cur = slist->header;

    if (cur_lvl < 0){
        return NULL;
    }
/*
    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++){
        prevs[i] = cur;
    }
  */  
    for (;cur_lvl >= 0; cur_lvl--){
        nxt = cur->links[cur_lvl].next;
        /*  The most time-consuming part (about half the execution time).
            If we can do some optimize, that will be a huge improvment.
            possible improvment:
            - No `prev` link, shrinking the size of `links` member by half.
        */
        for (;nxt != NULL && nxt->key < key;){
            /* Proceed until we can't. */
            cur = nxt;
            nxt = cur->links[cur_lvl].next;
        }
        assert(nxt == NULL || nxt->key >= key);

        /* record `cur` as prev and down the level. */
        prevs[cur_lvl] = cur;
    }
    if (nxt != NULL && nxt->key == key){
        found = nxt;
    }


    return found;
}

int skiplist_get(struct skiplist *slist, int key, int *res){
    int found = -1;
    struct skiplist_node *prevs[SKIPLIST_MAX_LEVEL];
    struct skiplist_node *target = NULL;

    target = skiplist_get_ex(slist, key, prevs);

    // "inlining" the logic does not have much impact on time.
    /*
    struct skiplist_node *prv = slist->header, *nxt;
    for (int i = slist->current_max_level; i >= 0; i--){
        nxt = prv->links[i].next;
        for (;nxt != NULL && nxt->key < key;){
            prv = nxt;
            nxt = nxt->links[i].next;
        }
    }
    if (nxt != NULL && nxt->key == key){
        *res = nxt->val;
        return 0;
    }
    return -1;
*/

    if (target != NULL){
        *res = target->val;
        found = 0;
    }
    
    return found;
}

int skiplist_set(struct skiplist *slist, int key, int val){
    int level;
    struct skiplist_node *prevs[SKIPLIST_MAX_LEVEL];
    struct skiplist_node *node = NULL;

    node = skiplist_get_ex(slist, key, prevs);
    /* Require a key is not set. */
    if (node != NULL){
        return -1;
    }
    
    level = generate_level();
    assert(level < SKIPLIST_MAX_LEVEL);
    node = slist_node_alloc(level);
    if (node == NULL){
        return -1;
    }
    if (level > slist->current_max_level){
        /* Don't forget to update! */
        slist->current_max_level = level;
    }
    assert(node->level == level);
    node->key = key;
    node->val = val;

    struct skiplist_node *prv, *nxt;
    for (int i = 0; i < (level+1); i++){
        prv = prevs[i];
        nxt = prv->links[i].next;
        assert(prv != NULL);

        node->links[i].next = nxt;
        prv->links[i].next = node;
    }

    return 0;
}

int skiplist_del(struct skiplist *slist, int key){
    struct skiplist_node *node = NULL;
    struct skiplist_node *prevs[SKIPLIST_MAX_LEVEL];

    node = skiplist_get_ex(slist, key, prevs);
    if (node == NULL){
        return -1; // ???
    }
    struct skiplist_node *prv, *nxt;
    for (int i = 0; i < (node->level+1); ++i){
        prv = prevs[i];
        nxt = node->links[i].next;

        prv->links[i].next = nxt;
    }
    free(node);

    return 0;
}