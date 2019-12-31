#include "rbtree.h"


/*
 *  Rewrite rbtree to intrusive version:
 *      - new nodes are prepared outside APIs.
 *      - no alloc and free inside API.
 */


enum chiral {
    CHIRAL_LEFT  = 0,
    CHIRAL_RIGHT = 1
};


static
int node_validate(struct rbtree *tree, struct rbtnode *node){
    int bh_l = 0, bh_r;
    if (node == tree->nil){
        return 1;
    }
    if (node->color == COLOR_RED){
        /* constrain #4 */
        assert(node->parent->color == COLOR_BLACK);
        assert(node->left->color   == COLOR_BLACK);
        assert(node->right->color  == COLOR_BLACK);
    }
    bh_l = node_validate(tree, node->left);
    bh_r = node_validate(tree, node->right);
    /* constrain #5 */
    assert(bh_l == bh_r);
    if (node->color == COLOR_BLACK){
        bh_l += 1;
    }
    return bh_l;
}

static
void left_rotate(struct rbtree *tree, struct rbtnode *x){
    struct rbtnode *y = x->right, *nil = tree->nil;
    // struct rbtnode **y = &(*x)->right;
    x->right = y->left;
    if (y->left != nil){
        y->left->parent = x;
    }
    y->parent = x->parent;

    if (x->parent == nil){
        tree->root = y;
    } else if (x == x->parent->left){
        x->parent->left = y;
    } else {
        //     x == x->parent->right
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
    return;
}

static
void right_rotate(struct rbtree *tree, struct rbtnode *x){
    struct rbtnode *y = x->left, *nil = tree->nil;
    x->left = y->right;
    if (y->right != nil){
        y->right->parent = x;
    }
    y->parent = x->parent;

    if (x->parent == nil){
        tree->root = y;
    } else if (x == x->parent->left){
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
    return;
}

/* TODO: tighten the code flow */
static
void insert_fix(struct rbtree *tree, struct rbtnode *node){
    struct rbtnode *uncle, *parent, *gparent;
    void (*rotator[2])(struct rbtree*, struct rbtnode*) = {
        left_rotate, right_rotate
    }; /* array of function pointers*/
    int chiral = 0, do_case2 = 0;
    for (;node->parent->color == COLOR_RED;){
        parent  = node->parent;
        gparent = parent->parent;
        if (parent == gparent->left){
            uncle    = gparent->right;
            chiral   = CHIRAL_LEFT;  /* left */
            do_case2 = (node == parent->right);
        } else {
            uncle    = gparent->left;
            chiral   = CHIRAL_RIGHT; /* right */
            do_case2 = (node == parent->left);
        }
        if (uncle->color == COLOR_RED){
            /* case #1 */
            parent->color  = COLOR_BLACK;
            uncle->color   = COLOR_BLACK;
            gparent->color = COLOR_RED;
            node = gparent;
            continue;
        }
        if (do_case2){
            /* case #2 */
            node    = node->parent;
            rotator[chiral](tree, node);
            parent  = node->parent;
            gparent = parent->parent;
        }
        /* case #3 */
        parent->color  = COLOR_BLACK;
        gparent->color = COLOR_RED;
        rotator[!chiral](tree, gparent);
    }
    tree->root->color = COLOR_BLACK;
    return;
}

static
void delete_fix(struct rbtree *tree, struct rbtnode *node){
    struct rbtnode *sibling, *parent;
    void (*rotator[2])(struct rbtree*, struct rbtnode*) = {
        left_rotate, right_rotate
    }; /* array of function pointers*/
    int chiral;
    struct rbtnode *nephews[2];
    for (;node != tree->root && rbt_is_black(node);){
        parent = node->parent;
        if (node == parent->left){
            sibling = parent->right;
            chiral  = CHIRAL_LEFT;
        } else {
            sibling = parent->left;
            chiral  = CHIRAL_RIGHT;
        }
        /*
         *  if the layout is compact, we can just assign the address of
         *  `sibling->left` to `nephews`.
         *  nephews = (struct rbtnode *[2])&sibling->left;
         */
        nephews[CHIRAL_LEFT]  = sibling->left;
        nephews[CHIRAL_RIGHT] = sibling->right;
        if (rbt_is_red(sibling)){
            /* case #1 */
            rbt_set_black(sibling);
            rbt_set_red(parent);
            rotator[chiral](tree, parent);
            if (chiral == CHIRAL_LEFT){
                sibling = parent->right;
            } else {
                sibling = parent->left;
            }
            nephews[CHIRAL_LEFT]  = sibling->left;
            nephews[CHIRAL_RIGHT] = sibling->right;
        }
        if (rbt_is_black(sibling->left) &&
            rbt_is_black(sibling->right)){
            /* case #2 */
            sibling->color = COLOR_RED;
            node = parent;
            continue;
        }
        if (nephews[!chiral]->color == COLOR_BLACK){
            /* case #3 */
            nephews[chiral]->color = COLOR_BLACK;
            rbt_set_red(sibling);
            rotator[!chiral](tree, sibling);
            if (chiral == CHIRAL_LEFT){
                sibling = parent->right;
            } else {
                sibling = parent->left;
            }
            nephews[CHIRAL_LEFT]  = sibling->left;
            nephews[CHIRAL_RIGHT] = sibling->right;
        }
        /* case #4 */
        sibling->color = parent->color;
        rbt_set_black(parent);
        nephews[!chiral]->color = COLOR_BLACK;
        rotator[chiral](tree, parent);
        node = tree->root;
    }
    rbt_set_black(node);
    return;
}

void rbtree_init(struct rbtree *tree, struct rbtnode *nil, rbt_cmp_func cmp){
    nil->left   = NULL;
    nil->right  = NULL;
    nil->parent = NULL;
    nil->color  = COLOR_BLACK;
    tree->count = 0;
    tree->cmp   = cmp;
    tree->nil   = nil;
    tree->root  = nil;
    return;
}

int rbtree_validate(struct rbtree *tree){
    /* constrain #2 */
    assert(tree->root->color == COLOR_BLACK);
    /* constrain #3 */
    assert(tree->nil->color  == COLOR_BLACK);
    /* constrain #4 #5 */
    node_validate(tree, tree->root);
    return 0;
}

int rbtree_set(struct rbtree *tree, struct rbtnode *node){
    struct rbtnode *parent, *nil = tree->nil, **indirect;
    int cmpres;
    indirect = &tree->root;
    parent = *indirect;
    for (;;){
        // this field is containing "NIL"
        if ((*indirect) == nil){
            /*
                we've found an empty place, implies no corresponding key
                was found.
            */
            *indirect    = node;
            node->parent = parent;
            node->left   = nil;
            node->right  = nil;
            tree->count++;
            insert_fix(tree, node);
            break;
        }
        /* assured (*indirect) is not NULL */
        /* preserve which way coming and proceed */
        parent = *indirect;
        cmpres = tree->cmp(node, *indirect);
        if (cmpres == CMP_LT){
            /* indirect points to the field instead */
            indirect = &(*indirect)->left;
        } else if (cmpres == CMP_GT){
            indirect = &(*indirect)->right;
        } else {
            /* we're pointing is what we're looking for */
            return RBT_EXISTED;
        }
    }
    return RBT_OK;
}

struct rbtnode *rbtree_get(struct rbtree *tree, struct rbtnode *hint){
    struct rbtnode *cur, *nil = tree->nil;
    int cmpres;
    cur = tree->root;
    for (;cur != nil;){
        cmpres = tree->cmp(hint, cur);
        if (cmpres == CMP_EQ){
            break;
        }
        if (cmpres == CMP_LT){
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    if (cur == nil){
        return NULL;
    }
    return cur;
}

void rbtree_delete(struct rbtree *tree, struct rbtnode *node){
    struct rbtnode
        **indirect, *subst,
        *orphan, *nil = tree->nil;

    if (node == tree->root){
        indirect = &tree->root;
    } else if (node == node->parent->left){
        indirect = &node->parent->left;
    } else {
        indirect = &node->parent->right;
    }
    /* --- just a direct copy from ngx_rbtree_delete */
    // maybe there is a way to shrink code size.
    if (node->left == nil){
        orphan = node->right;
        subst = node;
    } else if (node->right == nil){
        orphan = node->left;
        subst = node;
    } else {
        subst = rbtree_min(*indirect, nil);
        if (subst->left != nil){
            orphan = subst->left;
        } else {
            orphan = subst->right;
        }
    }

    if (subst == tree->root){
        tree->root = orphan;
        rbt_set_black(orphan);
        node->left   = NULL;
        node->right  = NULL;
        node->parent = NULL;
        return;
    }
    /* orphan replaces where subst be */
    if (subst == subst->parent->left){
        subst->parent->left = orphan;
    } else {
        subst->parent->right = orphan;
    }
    if (subst == node){
        /*  
         *  can't find subst other than node itself, node is
         *  leaf node.
         */
        orphan->parent = subst->parent;
    } else {
        /* 
         *  set parent of orphan as the "parent" of subst.
         */
        if (subst->parent == node){
            /* subst is direct child of node */
            /* 
             * subst is moving to where original parent is, so just
             * link it right away.
             */
            orphan->parent = subst;
        } else {
            orphan->parent = subst->parent;
        }
        /* copy attributes of node */
        subst->left   = node->left;
        subst->right  = node->right;
        subst->parent = node->parent;
        rbt_cpy_color(subst, node);

        /* substitute the place holding `node` with `subst` */
        *indirect = subst;

        if (subst->left != nil){
            subst->left->parent  = subst;
        }
        if (subst->right != nil){
            subst->right->parent = subst;
        }
    }
    node->left   = NULL;
    node->right  = NULL;
    node->parent = NULL;
    /* --- */
    if (node->color == COLOR_BLACK){
        delete_fix(tree, orphan);
    }
    tree->count--;
    return;
}
