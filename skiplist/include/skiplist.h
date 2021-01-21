#ifndef SKIPLIST_H
#define SKIPLIST_H


#define SKIPLIST_MAX_LEVEL 24


/* > Hide definition completly? */

struct skiplist_link {
    struct skiplist_node *next;
};

/* TODO: Move `links` to head and alloc separately. */
struct skiplist_node {
    int key, val;
    int level;
    struct skiplist_link links[]; // flex array member.
};

struct skiplist {
    int current_max_level;
    struct skiplist_node *header;
    /* debug */
    int dist[SKIPLIST_MAX_LEVEL];
    long long search_steps;
};


struct skiplist *skiplist_alloc(void);
void skiplist_free(struct skiplist *slist);
int skiplist_get(struct skiplist *slist, int key, int *res);
int skiplist_set(struct skiplist *slist, int key, int val);
int skiplist_del(struct skiplist *slist, int key);
/*
static int skiplist_get_ex(
    struct skiplist *slist, int key,
    struct skiplist *prevs[SKIPLIST_NAX_LEVEL]
); // This is the bottom layer of all skiplist API.
*/


#endif