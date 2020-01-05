#include "list.h"



void list_push(struct list *head, struct list *node){
    node->next = head->next;
    node->prev = head;
    head->next->prev = node;
    head->next = node;
    return;
}

struct list *list_pop(struct list *head){
    struct list *tmp;
    if (list_is_empty(head)) return NULL;
    tmp = head->next;
    head->next = tmp->next;
    tmp->next->prev = head;
    tmp->next = NULL;
    tmp->prev = NULL;
    return tmp;
}

void list_append(struct list *head, struct list *node){
    node->prev = head->prev;
    node->next = head;
    head->prev->next = node;
    head->prev = node;
    return;
}

struct list *list_get(struct list *head){
    struct list *tmp;
    if (list_is_empty(head)) return NULL;
    tmp = head->prev;
    head->prev = tmp->prev;
    tmp->prev->next = head;
    tmp->next = NULL;
    tmp->prev = NULL;
    return tmp;
}

void list_delete(struct list *node){
    node->prev->next = node->next;
    node->next->prev = node->prev;
    return;
}
