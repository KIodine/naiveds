#include "dllist.h"


static
struct dllnode *node_alloc(int val){
    struct dllnode *node;
    node = calloc(1, sizeof(struct dllnode));
    node->val = val;
    return node;
}

static
void node_purge(struct dllnode *node){
    struct dllnode *hold;
    for (struct dllnode *n = node->next; n != node;){
        hold = n;
        n = n->next;
        free(hold);
    }
    node->next = node;
    node->prev = node;
    return;
}

void dll_purge(struct dllist *list){
    list->count = 0;
    node_purge(&list->first);
    return;
}

struct dllist *dll_alloc(void){
    struct dllist *list;
    struct dllnode *sentinel;
    list = calloc(1, sizeof(struct dllist));
    sentinel = &list->first;
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    // the first node is used as sentinel node and thus
    // holds no data.
    return list;
}

void dll_free(struct dllist *list){
    list->count = 0;
    node_purge(&list->first);
    free(list);
    return;
}

int dll_push(struct dllist *list, int val){
    struct dllnode *sentinel = &list->first, *old_next, *new;
    old_next = sentinel->next;
    new = node_alloc(val);
    new->next = old_next;
    new->prev = sentinel;
    old_next->prev = new;
    sentinel->next = new;
    return DLL_OK;
}

int dll_pop(struct dllist *list, int *res){
    struct dllnode *sentinel = &list->first, *hold;
    if (sentinel->next == sentinel){
        return DLL_NOELEM;
    }
    hold = sentinel->next;
    sentinel->next = hold->next;
    hold->next->prev = sentinel;
    *res = hold->val;
    free(hold);
    return DLL_OK;
}

int dll_append(struct dllist *list, int val){
    struct dllnode *sentinel = &list->first, *new, *old_prev;
    old_prev = sentinel->prev;
    new = node_alloc(val);
    new->next = sentinel;
    new->prev = old_prev;
    old_prev->next = new;
    sentinel->prev = new;
    return DLL_OK;
}

int dll_get(struct dllist *list, int *res){
    struct dllnode *sentinel = &list->first, *hold;
    if (sentinel->prev == sentinel){
        return DLL_NOELEM;
    }
    hold = sentinel->prev;
    sentinel->prev = hold->prev;
    hold->prev->next = sentinel;
    *res = hold->val;
    free(hold);
    return DLL_OK;
}

int dll_delete(struct dllist *list, int val){
    struct dllnode* sentinel = &list->first, *cur, *prev;
    for (cur = sentinel->next; cur != sentinel; cur = cur->next){
        if (cur->val == val){
            prev = cur->prev;
            prev->next = cur->next;
            cur->next->prev = prev;
            free(cur);
            break;
        }
    }
    return cur != sentinel? DLL_OK: DLL_NOELEM;
}
