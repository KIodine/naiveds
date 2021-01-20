#ifndef SKIPLIST_H
#define SKIPLIST_H


#define SKIPLIST_MAX_LEVEL 16
#define SKIPLIET_UPGRADE_RATIO (4096)


/* > Hide definition completly? */

struct skiplist_link {
    struct skiplist_node *prev, *next;
};

struct skiplist_node {
    int key, val;
    int level;  // probably not necessary.
    struct skiplist_link links[]; // flex array member.
};

struct skiplist {
    int current_max_level;
    struct skiplist_node *header;
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