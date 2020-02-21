#include "avltree.h"

static inline int max(int a, int b);
static inline int min(int a, int b);
static struct avlnode *node_min(struct avlnode *node);
static void left_rotate(struct avltree *tree, struct avlnode *a);
static void right_rotate(struct avltree *tree, struct avlnode *a);


static inline int max(int a, int b){
    return (a > b)? a: b;
}

static inline int min(int a, int b){
    return (b > a)? a: b;
}

static struct avlnode *node_min(struct avlnode *node){
    if (node == NULL){
        return NULL;
    }
    for (;node->child[CLD_L] != NULL;){
        node = node->child[CLD_L];
    }
    return node;
}

static void left_rotate(struct avltree *tree, struct avlnode *a){
    struct avlnode *b, *beta, *parent, **rootp;
    b      = a->child[CLD_R];
    beta   = b->child[CLD_L];
    parent = a->parent;
    rootp  = &tree->root;

    a->child[CLD_R] = beta;
    if (beta != NULL){
        beta->parent = a;
    }
    if (a->parent != NULL){
        if (a == parent->child[CLD_L]){
            b = parent->child[CLD_L];
        } else {
            b = parent->child[CLD_R];
        }
    } else {
        *rootp = b;
    }
    b->parent = parent;
    b->child[CLD_L] = a;
    a->parent = b;

    a->factor = a->factor - max(b->factor, 0) - 1;
    b->factor = b->factor + min(a->factor, 0) - 1;
    return;
}

static void right_rotate(struct avltree *tree, struct avlnode *a){
    struct avlnode *b, *beta, *parent, **rootp;
    b      = a->child[CLD_L];
    beta   = b->child[CLD_R];
    parent = a->parent;
    rootp  = &tree->root;

    a->child[CLD_L] = beta;
    if (beta != NULL){
        beta->parent = a;
    }
    if (a->parent != NULL){
        if (a == parent->child[CLD_L]){
            b = parent->child[CLD_L];
        } else {
            b = parent->child[CLD_R];
        }
    } else {
        tree->root = b;
    }
    b->parent = parent;
    b->child[CLD_R] = a;
    a->parent = b;

    a->factor = a->factor - min(b->factor, 0) + 1;
    b->factor = b->factor + max(a->factor, 0) + 1;
    return;
}


void avl_tree_init(struct avltree *tree, avl_cmp_t cmp){
    tree->root  = NULL;
    tree->cmp   = cmp;
    tree->count = 0;
    return;
}

void avl_node_init(struct avlnode *node){
    memset(node, 0, sizeof(struct avlnode));
    return;
}

int avl_insert(struct avltree *tree, struct avlnode *node){
    int cmpres;
    struct avlnode **tmp, **rootp, *parent = NULL;

    tmp = &tree->root;
    rootp = tmp;

    for (;(*tmp) != NULL;){
        cmpres = tree->cmp(node, (*tmp));
        parent = *tmp;
        if (cmpres < 0){
            tmp = &(*tmp)->child[CLD_L];
        } else if (cmpres > 0){
            tmp = &(*tmp)->child[CLD_R];
        } else {
            break;
        }
    }
    if ((*tmp) != NULL){
        // place have been occupied
        return -1;
    }
    
    *tmp = node;
    if ((*tmp) == *rootp){
        // special case: insert at root
        return 0;
    }
    node->parent = parent;
    
    // do fix
    struct avlnode *child = node;
    int is_same_dir;
    for (;parent != NULL;){
        // FIXME: don't use node, determine child after inbalance is
        //        detected.
        if (child == parent->child[CLD_L]){
            parent->factor -= 1;
        } else {
            parent->factor += 1;
        }
        if (abs(parent->factor) < 2){
            // step upward
            child = parent;
            parent = child->parent;
            continue;
        }
        if (parent->factor < 0){
            child = parent->child[CLD_L];
        } else {
            child = parent->child[CLD_R];
        }
        is_same_dir = (child->factor < 0) == (parent->factor < 0);
        if (child->factor != 0 && !is_same_dir){
            // inbalabce and not the same case as parent
            if (child->factor < 0){
                // RL case
                right_rotate(tree, child);
            } else {
                // LR case
                left_rotate(tree, child);
            }
            // after rotation, node has descended for one level
            child = child->parent;
        }
        if (parent->factor < 0){
            right_rotate(tree, parent);
        } else {
            left_rotate(tree, parent);
        }
        // it is guarenteed after one fix, the tree must be a valid AVL
        // tree (but can update factor up to log n).
        break;
    }

    return 0;
}

struct avlnode *avl_get(struct avltree *tree, struct avlnode *hint){
    int cmpres;
    struct avlnode *res = tree->root;
    for (;res != NULL;){
        cmpres = tree->cmp(res, hint);
        if (cmpres < 0){
            res = res->child[CLD_L];
        } else if (cmpres > 0){
            res = res->child[CLD_R];
        } else {
            break;
        }
    }
    return res;
}

int avl_delete(struct avltree *tree, struct avlnode *node){
    int cmpres;
    struct avlnode *tmp, *parent, *fix;
    
    parent = node->parent;
    if (node->child[CLD_L] == NULL){
        tmp = node->child[CLD_R];
        parent->factor -= 1;
        fix = parent;
    } else if (node->child[CLD_R] == NULL){
        tmp = node->child[CLD_L];
        parent->factor += 1;
        fix = parent;
    } else {
        // 2 child case, tmp is guarenteed to be non-NULL
        tmp = node_min(node->child[CLD_R]);
        if (tmp != node->child[CLD_R]){
            // is not direct child, we have to link the "orphan"
            fix = tmp->parent;
            tmp->parent->factor += 1;
            tmp->parent->child[CLD_L] = tmp->child[CLD_R];
            if (tmp->child[CLD_R] != NULL){
                tmp->child[CLD_R]->parent = tmp->parent;
            }
            tmp->child[CLD_R] = node->child[CLD_R];
            node->child[CLD_R]->parent = tmp;
        } else {
            fix = tmp;
            tmp->factor -= 1;
        }
        tmp->child[CLD_L] = node->child[CLD_L];
        node->child[CLD_L]->parent = tmp;
    }
    // link parent and tmp
    if (tmp != NULL){
        // could from one child case
        tmp->parent = parent;    
    }
    if (parent != NULL){
        if (node == parent->child[CLD_L]){
            parent->child[CLD_L] = tmp;
        } else {
            parent->child[CLD_R] = tmp;
        }
    } else {
        tree->root = tmp;
    }
    // do fixup

    struct avlnode *child = NULL;
    int is_same_dir, is_child_rotate;
    // child may be NULL, so we mix the first factor update in previous
    // code sections

    for (;fix != NULL;){
        if (abs(fix->factor) < 2){
            break;
        }
        if (fix->factor > 0){
            child = fix->child[CLD_R];
        } else if (fix->factor < 0) {
            child = fix->child[CLD_L];
        } else {
            child = NULL;
        }
        if (child != NULL){
            is_same_dir = (fix->factor > 0) == (child->factor > 0);
            is_child_rotate = (child->factor != 0);
            // note that `is_same_dir` doesn't cover the case
            // when `child->factor == 0`.
            if (!is_same_dir && is_child_rotate){
                if (child->factor > 0){
                    left_rotate(tree, child);
                } else {
                    right_rotate(tree, child);
                }
                // child have descended for one level, fix that
                child = child->parent;
            }
        }
        if (fix->factor > 0){
            left_rotate(tree, fix);
        } else {
            right_rotate(tree, fix);
        }
        // since fix descended for one level, fix that
        fix = fix->parent;
        
        if (fix->parent == NULL || !is_child_rotate){
            // fix is root or handled pure L or R case
            break;
        }
        // after LL LR RL RR fix, the height is descended, change
        // propagates.
        if (fix == fix->parent->child[CLD_L]){
            fix->parent->factor += 1;
        } else {
            fix->parent->factor -= 1;
        }
        child = fix;
        fix   = fix->parent;
    }
    return 0;
}
