#ifndef SLLIST_H
#define SLLIST_H


#include <stdlib.h>
#include <stdint.h>


struct sllnode {
    int val;
    struct sllnode *next;
};

struct sllist {
    int count;
    struct sllnode *first;
};

enum {
    SLL_OK,
    SLL_NOELEM,
};

struct sllist *sll_alloc(void);
void sll_free(struct sllist *list);
void sll_purge(struct sllist *list);

int sll_push(struct sllist *list, int val);
int sll_append(struct sllist *list, int val);
int sll_insert(struct sllist *list, unsigned int pos, int val);
int sll_get(struct sllist *list, unsigned int pos, int *res);
int sll_pop(struct sllist *list, int *res);
int sll_delete(struct sllist *list, int val);

void sll_reverse(struct sllist *list);

#define LIST_TRAVERSE(list, sym) \
    for (struct sllnode *sym = list->first; sym != NULL; sym = sym->next)


#endif