#include "sllist.h"

static
struct sllnode *node_alloc(int val){
    struct sllnode *node = NULL;
    node = calloc(1, sizeof(struct sllnode));
    node->val = val;
    return node;
}

static
void node_purge(struct sllist *list){
    struct sllnode *hold, *cur;
    cur = list->first;
    for (;cur != NULL;){
        hold = cur;
        cur = cur->next;
        free(hold);
    }
    list->first = NULL;
    return;
}

struct sllist *sll_alloc(void){
    struct sllist *list = NULL;
    list = calloc(1, sizeof(struct sllist));
    return list;
}

void sll_free(struct sllist *list){
    node_purge(list);
    free(list);
    return;
}

void sll_purge(struct sllist *list){
    node_purge(list);
    list->count = 0;
    return;
}

int sll_push(struct sllist *list, int val){
    struct sllnode *node = NULL;
    node = node_alloc(val);
    node->next = list->first;
    list->first = node;
    list->count++;
    return SLL_OK;
}

int sll_append(struct sllist *list, int val){
    struct sllnode **indirect;
    indirect = &list->first;
    for (;*indirect != NULL;){
        // just get where our target is.
        indirect = &(*indirect)->next;
    }
    /* go on until we have a empty field (regardless who holds the field) */
    *indirect = node_alloc(val);
    list->count++;
    return SLL_OK;
}

int sll_insert(struct sllist *list, unsigned int pos, int val){
    struct sllnode **indirect, *new;
    new = node_alloc(val);
    indirect = &list->first;
    for (unsigned int i = 0; i < pos && *indirect != NULL; ++i){
        indirect = &(*indirect)->next;
    }
    new->next = (*indirect);
    *indirect = new;
    list->count++;
    return SLL_OK;
}

int sll_get(struct sllist *list, unsigned int pos, int *res){
    struct sllnode **indirect;
    indirect = &list->first;
    for (unsigned int i = 0; i < pos && *indirect != NULL; ++i){
        indirect = &(*indirect)->next;
    }
    if (*indirect == NULL){
        return SLL_NOELEM;
    }
    *res = (*indirect)->val;
    return SLL_OK;
}

int sll_pop(struct sllist *list, int *res){
    struct sllnode **indirect, *hold;

    indirect = &list->first;
    if (*indirect == NULL){
        return SLL_NOELEM;
    }
    hold = *indirect;
    *res = hold->val;
    *indirect = (*indirect)->next;
    free(hold);
    list->count--;
    return SLL_OK;
}

int sll_delete(struct sllist *list, int val){
    struct sllnode **indirect, *hold;
    indirect = &list->first;
    for (;*indirect != NULL && (*indirect)->val != val;){
        indirect = &(*indirect)->next;
    }
    if (*indirect == NULL){
        return SLL_NOELEM;
    }
    hold = *indirect;
    *indirect = (*indirect)->next;
    free(hold); // it might be NULL, but this is ok.
    return SLL_OK;
}

void sll_reverse(struct sllist *list){
    struct sllnode **indirect, *new_prev, *new_next;
    indirect = &list->first;
    new_next = NULL;    // the next of new end is always NULL
    for (;*indirect != NULL;){
        new_prev = (*indirect)->next;   // preserve link to next
        (*indirect)->next = new_next;   // link to "prev"
        new_next = *indirect;           // step new_next
        if (new_prev == NULL){
            break;                      // reached "old" end
        }
        *indirect = new_prev;           // go to next node
    }
    return;
}
