#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>


// copy from <linux/kernel.h> and strip debug stuffs.
#define container_of(ptr, type, member) ({			\
	void *__mptr = (void *)(ptr);					\
	((type *)(__mptr - offsetof(type, member))); })


struct map;
struct map_node;


typedef uint32_t (*obj_hasher)(struct map_node*);
typedef int (*obj_cmp)(struct map_node*, struct map_node*);

struct map_node {
    uint32_t hashval;
    struct map_node **pprev, *next;
};
/* `**pprev` means `where did this object put` */

struct map {
    obj_hasher        hasher;
    obj_cmp           cmp;
    unsigned int      capacity;
    unsigned int      count;
    unsigned int      resize_thres;
    struct map_node **bucket;
};


struct map *map_alloc(unsigned int size_hint, obj_hasher hash, obj_cmp cmp);
void map_free(struct map *m);

int map_insert(struct map *m, struct map_node *mnode);
struct map_node *map_get(struct map *m, struct map_node *hint);
int map_delete(struct map *m, struct map_node *mnode);


#endif /* HASHMAP_H */