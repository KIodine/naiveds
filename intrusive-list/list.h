#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#define list_entry(ptr, type, member) ({ \
    void *__tmp = (void*)(ptr);\
    (type *)((char*)(__tmp) - offsetof(type, member))\
})


struct list {
    struct list *prev, *next;
};

#define list_decl(ident) struct list ident = {&(ident), &(ident)}
#define list_is_empty(node) ((node)->prev == (node)->next)

void         list_push(struct list *head, struct list *node);
struct list *list_pop(struct list *head);
void         list_append(struct list *head, struct list *node);
struct list *list_get(struct list *head);
void         list_delete(struct list *node);

#define list_traverse(head, sym) \
    for (struct list *(sym) = (head)->next; (sym) != (head); (sym) = (sym)->next)

#endif