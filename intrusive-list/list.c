#include "list.h"



void list_push(struct list *head, struct list *node){
    struct list *hnxt;
    hnxt = head->next;

    node->next = hnxt;
    node->prev = head;
    hnxt->prev = node;
    head->next = node;
    return;
}

struct list *list_pop(struct list *head){
    struct list *tmp, *tnxt;
    if (list_is_empty(head)) return NULL;
    tmp = head->next;
    tnxt = tmp->next;
    
    head->next = tnxt;
    tnxt->prev = head;
    tmp->next  = tmp;
    tmp->prev  = tmp;
    return tmp;
}

void list_append(struct list *head, struct list *node){
    struct list *hprv;
    hprv = head->prev;

    node->prev = hprv;
    node->next = head;
    hprv->next = node;
    head->prev = node;
    return;
}

struct list *list_get(struct list *head){
    struct list *tmp, *tprv;
    if (list_is_empty(head)) return NULL;
    tmp = head->prev;
    tprv = tmp->prev;
    
    head->prev = tprv;
    tprv->next = head;
    tmp->next  = tmp;
    tmp->prev  = tmp;
    return tmp;
}

void list_delete(struct list *node){
    struct list *nprv, *nnxt;
    nprv = node->prev;
    nnxt = node->next;
    nprv->next = nnxt;
    nnxt->prev = nprv;
    node->prev = node->next = node;
    return;
}
