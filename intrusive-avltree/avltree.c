#include "avltree.h"

static inline int max(int a, int b);
static inline int min(int a, int b);
static struct avlnode *node_min(struct avlnode *node);
static void left_rotate(struct avltree *tree, struct avlnode *a);
static void right_rotate(struct avltree *tree, struct avlnode *a);

static void node_print(struct avlnode *node, int depth);
static void node_validate(struct avlnode *node, avl_cmp_t cmp);

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

// FIXME: rotation is broken, seems balance factor calulation is the key.
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
    
    if (parent != NULL){
        if (a == parent->child[CLD_L]){
            parent->child[CLD_L] = b;
        } else {
            parent->child[CLD_R] = b;
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
    
    if (parent != NULL){
        if (a == parent->child[CLD_L]){
            parent->child[CLD_L] = b;
        } else {
            parent->child[CLD_R] = b;
        }
    } else {
        *rootp = b;
    }
    b->parent = parent;
    
    b->child[CLD_R] = a;
    a->parent = b;

    a->factor = a->factor - min(b->factor, 0) + 1;
    b->factor = b->factor + max(a->factor, 0) + 1;
    return;
}

int avl_insert(struct avltree *tree, struct avlnode *node){
    int cmpres;
    struct avlnode **tmp, **rootp, *fix = NULL;

    tmp = &tree->root;
    rootp = tmp;

    assert(node->factor == 0);
    for (;(*tmp) != NULL;){
        cmpres = tree->cmp(node, (*tmp));
        fix = *tmp;
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
    node->parent = fix;
    
    // do fix
    struct avlnode *child = node;
    int is_same_dir, tmp_factor;
    for (;fix != NULL;){
        tmp_factor = fix->factor;

        if (child == fix->child[CLD_L]){
            tmp_factor -= 1;
        } else {
            tmp_factor += 1;
        }
        fix->factor = tmp_factor;
        if (tmp_factor == 0){
            // the insertion balances the tree, bail out
            return 0;
        }
        assert(tmp_factor != 0);
        if (abs(tmp_factor) < 2){
            // step upward
            child = fix;
            fix = fix->parent;
            continue;
        }
        assert(tmp_factor != 0 && abs(tmp_factor) == 2);
        if (fix->factor < 0){
            child = fix->child[CLD_L];
        } else {
            child = fix->child[CLD_R];
        }
        is_same_dir = (child->factor < 0) == (fix->factor < 0);
        if (child->factor != 0 && !is_same_dir){
            // inbalabce and not the same direction as parent
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
        if (fix->factor < 0){
            right_rotate(tree, fix);
        } else {
            left_rotate(tree, fix);
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
        cmpres = tree->cmp(hint, res);
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
    struct avlnode *tmp, *parent, *fix;
    
    parent = node->parent;
    if (node->child[CLD_L] == NULL){
        tmp = node->child[CLD_R];
        
        if (tmp){
            fix = tmp;
            assert(fix->factor == 0);
        } else {
            fix = parent;
            if (node == fix->child[CLD_L]){
                fix->factor += 1;
            } else {
                fix->factor -= 1;
            }
        }

    } else if (node->child[CLD_R] == NULL){
        tmp = node->child[CLD_L];
        
        fix = parent;

        if (tmp){
            fix = tmp;
            assert(fix->factor == 0);
        } else {
            fix = parent;
            if (node == fix->child[CLD_L]){
                fix->factor += 1;
            } else {
                fix->factor -= 1;
            }
        }

    } else {
        // 2 child case, tmp is guarenteed to be non-NULL
        tmp = node_min(node->child[CLD_R]);
        assert(tmp->child[CLD_L] == NULL);
        if (tmp != node->child[CLD_R]){
            // is not direct child, we have to link the "orphan"
            tmp->factor = node->factor;
            fix = tmp->parent;
            fix->factor += 1;
            tmp->parent->child[CLD_L] = tmp->child[CLD_R];
            if (tmp->child[CLD_R] != NULL){
                tmp->child[CLD_R]->parent = tmp->parent;
            }
            tmp->child[CLD_R] = node->child[CLD_R];
            node->child[CLD_R]->parent = tmp;
        } else {
            // is direct child
            if (tmp->child[CLD_R] != NULL){
                fix = tmp->child[CLD_R];
                fix->factor = tmp->factor;
            } else {
                fix = tmp;
            }
            tmp->factor = node->factor;
            fix->factor -= 1;

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

    // FIXME: 
    //      do fix if `fix->factor == 0`
    //      keep update height until one of:
    //      - root is reached
    //      - proper fix is applied
    //      - the fix node just become inbalance/skew*

    if (fix->factor != 0 && abs(fix->factor) < 2){
        return 0;
    }

    struct avlnode *child = NULL;
    int is_same_dir, is_child_rotate;
    // child may be NULL, so we mix the first factor update in previous
    // code sections

    for (;fix != NULL;){
        is_child_rotate = 0;
        // maybe we jump to lable `update`

        if (fix->factor == 0){
            goto update;
        }
        // factor is non-zero
        if (abs(fix->factor) < 2){
            break;
        }
        // |factor| is ge 2 and non-zero
        assert(fix->factor != 0);
        assert(abs(fix->factor) == 2);
        if (fix->factor > 0){
            child = fix->child[CLD_R];
        } else {
            child = fix->child[CLD_L];
        }
        // double heavy or single heavy
        assert(child != NULL);
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
        if (fix->factor > 0){
            left_rotate(tree, fix);
        } else {
            right_rotate(tree, fix);
        }
        // since fix descended for one level, fix that
        fix = fix->parent;
        
        if (!is_child_rotate){
            break;
        }
    update:
        // LOGIC error: the `is_child_rotate` flag erroneously let
        // the `update` case breaks.
        if (fix->parent == NULL){
            // fix is root or fixed pure L or R case
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
    node->child[CLD_L] = NULL;
    node->child[CLD_R] = NULL;
    node->factor = 0;
    node->parent = NULL;
    return 0;
}


void avl_print(struct avltree *tree){
    putchar('\n');
    node_print(tree->root, 0);
    return;
}

#define NL "\n"
static
void node_print(struct avlnode *node, int depth){
    if (node == NULL){
        return;
    }
    node_print(node->child[CLD_R], depth + 1);
    for (int i = 0; i < depth; ++i){
        printf("    ");
    }
    printf("[%d] <%p>"NL, node->factor, (void*)node);
    node_print(node->child[CLD_L], depth + 1);
    return;
}
#undef NL

void avl_validate(struct avltree *tree){
    node_validate(tree->root, tree->cmp);
    return;
}

static
void node_validate(struct avlnode *node, avl_cmp_t cmp){
    int res;
    if (node == NULL){
        return;
    }
    // AVL basic property
    assert(abs(node->factor) < 2);
    // BST basic property
    if (node->child[CLD_L] && node->child[CLD_R]){
        res = cmp(node->child[CLD_L], node->child[CLD_R]);
        assert(res < 0);
    }
    // more strict basic attribute test
    if (!node->child[CLD_L] && !node->child[CLD_R]){
        assert(node->factor == 0);
    }
    if (node->child[CLD_L] != NULL){
        assert(node->child[CLD_L]->parent = node);
    }
    if (node->child[CLD_R] != NULL){
        assert(node->child[CLD_R]->parent = node);
    }
    node_validate(node->child[CLD_L], cmp);
    node_validate(node->child[CLD_R], cmp);
    return;
}