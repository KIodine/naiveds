#ifndef UNIONFIND_H
#define UNIONFIND_H


struct ufnode {
    struct ufnode *parent;
    int rank;
};


void uf_init_set(struct ufnode *node);
struct ufnode *uf_find(struct ufnode *node);
int uf_connected(struct ufnode *a, struct ufnode *b);
int uf_union(struct ufnode *a, struct ufnode *b);


#endif
