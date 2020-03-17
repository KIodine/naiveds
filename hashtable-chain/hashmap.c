#include "hashmap.h"

#define NL "\n"
#define debug_print(fmt, ...) printf("[hashmap]" fmt, __VA_ARGS__);
#define debug_msg(s)          printf("[hashmap]" s);


static void map_init(struct map *m, unsigned int size_hint,
                     obj_hasher hash, obj_cmp cmp);
static struct map_node **map_find_slot(struct map_node **bucket,
                                struct map_node *mnode, uint32_t hashval,
                                obj_cmp cmp);                     
static int  map_is_need_grow(struct map *map);
static void map_resize(struct map *map);


static inline unsigned int round_up_pow_2(unsigned int v);
static inline void node_insert(struct map_node **slot, struct map_node *ins);


static inline
unsigned int round_up_pow_2(unsigned int v){
    /*
     *  fill bits lower than the leftmost bit.
     *  the decrement and increment prevents numbers already are
     *  power or 2 doubles.
     */
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

struct map *map_alloc(unsigned int size_hint, obj_hasher hash, obj_cmp cmp){
    struct map *m;
    m = malloc(sizeof(struct map));
    map_init(m, size_hint, hash, cmp);
    return m;
}

static
void map_init(
        struct map *m, unsigned int size_hint, obj_hasher hash,
        obj_cmp cmp
    ){
    unsigned int size, thres;
    size = round_up_pow_2(size_hint);
    thres = ((size + 1)*3)/4; /* 75% full */
    debug_print("rounding %u to %u"NL, size_hint, size);
    debug_print("threshold is %u"NL, thres);
    
    m->bucket = calloc(size, sizeof(struct map_node*));
    m->hasher = hash;
    m->cmp    = cmp;
    m->count  = 0;
    m->capacity     = size;
    m->resize_thres = thres;
    return;
}

static
int map_is_need_grow(struct map *m){
    return m->count >= m->resize_thres;
}

void map_free(struct map *m){
    free(m->bucket);
    free(m);
    return;
}

static
void map_resize(struct map *m){
    struct map_node **new_bkt, **old_bkt, **tmp_bkt, **slot,
                     *tmp_nd, *hold;
    unsigned int new_size, old_size, thres;
    uint32_t hashval;

    old_size = m->capacity;
    new_size = m->capacity << 1;
    thres = ((new_size + 1)*3)/4;
    new_bkt = calloc(new_size, sizeof(struct map_node*));
    old_bkt = m->bucket;

    debug_print("resizing map to %u (was %u)"NL, new_size, old_size);
    debug_print("new threshold is %u"NL, thres);
    debug_msg(">>>start resize"NL);

    for (unsigned int i = 0; i < old_size; ++i){
        tmp_nd = old_bkt[i];
        for (;tmp_nd != NULL;){
            hold = tmp_nd->next;
            hashval = tmp_nd->hashval;

            tmp_bkt = &new_bkt[hashval%new_size];
            
            /* clean the links */
            tmp_nd->next  = NULL;
            tmp_nd->pprev = NULL;

            slot = map_find_slot(tmp_bkt, tmp_nd, hashval, m->cmp);
            assert(*slot == NULL);
            node_insert(slot, tmp_nd);

            tmp_nd = hold;
        }
    }

    m->bucket = new_bkt;
    m->capacity = new_size;
    m->resize_thres = thres;

    free(old_bkt);
    /*
    alloc new bucket
    for every node in old bucket:
        get new bucket index
        find slot for new bucket
        insert node into slot
    free old bucket
    update size
    update threshold
    */

    debug_print("<<<resize to %u completed"NL, new_size);

    return;
}

static
struct map_node **map_find_slot(
    struct map_node **bucket, struct map_node *mnode, uint32_t hashval,
    obj_cmp cmp
){
    struct map_node **tmp = NULL;
    
    /*
        proceed until:
        1) no next node
        2) current node equals mnode(hint)
    */
    tmp = bucket;
    for (;(*tmp) != NULL;){
        if ((*tmp)->hashval == hashval){
            if (cmp(*tmp, mnode) != 0){
                break;
            }
        }
        debug_print("collision detected, val=%u"NL, hashval);
        tmp = &(*tmp)->next;
    }
    return tmp;
}

static inline
void node_insert(struct map_node **slot, struct map_node *ins){
    *slot = ins;
    ins->pprev = slot;
    return;
}

int map_insert(struct map *m, struct map_node *mnode){
    uint32_t hashval = 0;
    int is_existed   = 0;
    struct map_node **bucket;
    struct map_node **tmp_nd;

    hashval = m->hasher(mnode);
    mnode->hashval = hashval;
    
    bucket = &m->bucket[hashval%m->capacity];
    
    tmp_nd = map_find_slot(bucket, mnode, hashval, m->cmp);

    is_existed = ((*tmp_nd != NULL));

    if (!is_existed){
        node_insert(tmp_nd, mnode);
        m->count++;
        /* grow map if need */
        if (map_is_need_grow(m)){
            debug_print("count %u >= thres %u, now grow"NL, m->count, m->resize_thres);
            map_resize(m);
        }
    }

    return -is_existed;
}

struct map_node *map_get(struct map *m, struct map_node *hint){
    uint32_t hashval = 0;
    struct map_node **bucket;
    struct map_node **tmp_nd;

    hashval = m->hasher(hint);
    bucket = &m->bucket[hashval%m->capacity];

    tmp_nd = map_find_slot(bucket, hint, hashval, m->cmp);

    return *tmp_nd;
}

/*
delete:
    remove node
*/
int map_delete(struct map *m, struct map_node *mnode){
    *mnode->pprev = mnode->next;
    mnode->next  = NULL;
    mnode->pprev = NULL;
    
    m->count--;
    return 0;
}
