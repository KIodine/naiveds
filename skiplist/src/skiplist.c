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
    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++){
        /* approximately 1/4 ~= 0.25 */
        // or better implementation?
        if (rand() < RAND_MAX/4){
            level++;
        }
        break;
    }
    return level;
}

static struct skiplist_node *slist_node_alloc(int level){
    struct skiplist_node *snode = NULL;

    if (level < 0){
        return NULL;
    }

    snode = calloc(
        1, sizeof(struct skiplist_node)+sizeof(struct skiplist_link)*(level+1)
    );
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
    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++){
        header->links[i].next = header;
        header->links[i].prev = header;
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
    for (;cur != slist->header;){
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
    struct skiplist_node *const header = slist->header;
    int cur_lvl = slist->current_max_level;
    //int found = 0;

    cur = slist->header;
    nxt = cur->links[cur_lvl].next;

    if (cur_lvl < 0){
        return NULL;
    }

    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++){
        prevs[i] = cur;
    }
    /*
        loop:
            if nxt is valid and less than target:
                goto nxt
                continue
            if nxt is valid and nxt equals to target
                set found = 1
            assert nxt is null or nxt is greater than target
            record nxt as prev
            if no level to down
                break
            down the level, select new nxt
        if nxt is not null and nxt is target:
            found = 1
    */
    
    /* TODO: Handle when max level is invalid(<0). */
    for (;;){
        assert(nxt != NULL);
        if (nxt->key < key && nxt != header ){
        //if (nxt != header && nxt->key < key){
            /*  if `nxt` is valid and not greater than target, do
                follow the link. */
            cur = nxt;
            nxt = cur->links[cur_lvl].next;
            continue;
        }
        /* If we reached end of list or nxt is greater than target. */
        assert(nxt == header || nxt->key >= key);

        /* record `cur` as prev and down the level. */
        prevs[cur_lvl] = cur;
        cur_lvl--;
        if (cur_lvl < 0){
            break;
        }
        /* update `nxt`. */
        nxt = cur->links[cur_lvl].next;
    }
    if (nxt != header && nxt->key == key){
        found = nxt;
    }


    return found;
}

int skiplist_get(struct skiplist *slist, int key, int *res){
    int found = -1;
    struct skiplist_node *prevs[SKIPLIST_MAX_LEVEL];
    struct skiplist_node *target = NULL;

    target = skiplist_get_ex(slist, key, prevs);
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

        node->links[i].next = nxt;
        node->links[i].prev = prv;
        prv->links[i].next = node;
        nxt->links[i].prev = node;
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
    //printf("node.key=%d\n", node->key);
    //printf("node level=%d\n", node->level);
    struct skiplist_node *prv, *nxt;
    for (int i = 0; i < (node->level+1); ++i){
        prv = prevs[i];
        nxt = node->links[i].next;
        //printf("nxt_lvl=%d\n", nxt->level);
        assert(nxt->level >= i);

        prv->links[i].next = nxt;
        nxt->links[i].prev = prv; // Error: writing free'd memory?
        /* Seems `skiplist_get_ex` did not set prev correctly. */
    }
    free(node);

    return 0;
}