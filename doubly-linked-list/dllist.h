#ifndef DLLIST_H
#define DLLIST_H

#include <stdlib.h>


struct dllnode {
    int val;
    struct dllnode *prev, *next;
};

struct dllist {
    int count;
    struct dllnode first;
};

enum {
    DLL_OK,
    DLL_NOELEM
};

struct dllist *dll_alloc(void);
void dll_free(struct dllist *list);
void dll_purge(struct dllist *list);
int dll_delete(struct dllist *list, int val);
int dll_push(struct dllist *list, int val);
int dll_pop(struct dllist *list, int *res);
int dll_append(struct dllist *list, int val);
int dll_get(struct dllist *list, int *res);


#endif