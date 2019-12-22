#include "rbtree.h"


#ifdef __GNUC__
    #define FORCE_INLINE __attribute__((always_inline))
#elif
    #define FORCE_INLINE
#endif

enum chiral {
    CHIRAL_LEFT  = 0,
    CHIRAL_RIGHT = 1
};


static inline FORCE_INLINE
struct rbtnode *node_alloc(int key, int val){
    struct rbtnode *node = NULL;
    node = calloc(1, sizeof(struct rbtnode));
    node->key = key;
    node->val = val;
    return node;
}

static
struct rbtnode *rbtnode_alloc(struct rbtree *tree, int key, int val){
    struct rbtnode *node = NULL;
    node = node_alloc(key, val);
    /*
     * the color of node is naturally red because of the
     * order of enum order of COLOR_* constants.
     */
    node->color  = COLOR_RED;
    node->parent = tree->nil;
    node->left   = tree->nil;
    node->right  = tree->nil;
    return node;
}

static
void tree_purge(struct rbtnode* start, struct rbtnode *nil){
    if (start == nil){
        return;
    }
    tree_purge(start->left, nil);
    tree_purge(start->right, nil);
    free(start);
    return;
}

static
int node_validate(struct rbtree *tree, struct rbtnode *node){
    int bh_l = 0, bh_r;
    if (node == tree->nil){
        return 0;
    }
    if (node->color == COLOR_RED){
        /* constrain #4 */
        assert(node->parent->color == COLOR_BLACK);
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

/* TODO modify these functions */
static
struct rbtnode **find_precedence(
        struct rbtnode **start, struct rbtnode *nil
    ){
    /* find the rightmost node of left subtree */
    struct rbtnode **can;   /* the candidate */
    can = &(*start)->left;
    if (*can == nil){
        return NULL;        /* just for sentinel */
    }
    for (;(*can)->right != nil;){
        can = &(*can)->right;
    }
    return can;
}

static
struct rbtnode **find_successor(
        struct rbtnode **start, struct rbtnode *nil
    ){
    /* find the leftmost node of right subtree */
    struct rbtnode **can;
    can = &(*start)->right;
    if (*can == nil){
        return NULL;
    }
    /* only modify parent if candidate exists */
    for (;(*can)->left != nil;){
        can = &(*can)->left;
    }
    return can;
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

    // struct rbtnode **x;
    // *x = *y;
    /*
     * just modify the field holds x, in other words, x is one of:
     *  1) the address of root of tree
     *  2) the address of left or right child of parent
     */
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
    /*
     * By abusing the fact boolean in C is actually evaluate to
     * **0** and **1**, we can turn logic operation results into switch of
     * rotation functions.
     * Further, use **!<boolean>** to switch between 0 and 1.
     */
    /* TODO: cache nephews and access them use chiral */
    int chiral;
    struct rbtnode *nephews[2];
    for (;node != tree->root && node->color == COLOR_BLACK;){
        parent = node->parent;
        if (node == parent->left){
            sibling = parent->right;
            chiral  = CHIRAL_LEFT;
        } else {
            sibling = parent->left;
            chiral  = CHIRAL_RIGHT;
        }
        nephews[CHIRAL_LEFT]  = sibling->left;
        nephews[CHIRAL_RIGHT] = sibling->right;
        if (sibling->color == COLOR_RED){
            /* case #1 */
            sibling->color = COLOR_BLACK;
            parent->color  = COLOR_RED;
            rotator[chiral](tree, parent);
            if (chiral == CHIRAL_LEFT){
                sibling = parent->right;
            } else {
                sibling = parent->left;
            }
            nephews[CHIRAL_LEFT]  = sibling->left;
            nephews[CHIRAL_RIGHT] = sibling->right;
        }
        if (nephews[CHIRAL_LEFT]->color  == COLOR_BLACK &&
            nephews[CHIRAL_RIGHT]->color == COLOR_BLACK){
            /* case #2 */
            sibling->color = COLOR_RED;
            node = parent;
            continue;
        }
        if (nephews[chiral]->color == COLOR_RED){
            /* case #3 */
            nephews[chiral]->color = COLOR_BLACK;
            sibling->color = COLOR_RED;
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
        parent->color  = COLOR_BLACK;
        nephews[!chiral]->color = COLOR_BLACK;
        rotator[chiral](tree, parent);
        node = tree->root;
    }
    node->color = COLOR_BLACK;
    return;
}


struct rbtree *rbtree_alloc(void){
    struct rbtree *tree = NULL;
    struct rbtnode *nil = NULL;
    tree = calloc(1, sizeof(struct rbtree));
    nil = node_alloc(0, 0);
    nil->color  = COLOR_BLACK;
    nil->parent = nil;
    nil->left   = nil;
    nil->right  = nil;
    tree->nil   = nil;
    tree->root  = nil;
    return tree;
}

void rbtree_purge(struct rbtree *tree){
    tree_purge(tree->root, tree->nil);
    tree->root = tree->nil;
    tree->count = 0;
    return;
}

void rbtree_free(struct rbtree *tree){
    tree_purge(tree->root, tree->nil);
    free(tree->nil);
    free(tree);
    return;
}

int rbtree_validate(struct rbtree *tree){
    /* constrain #2 */
    assert(tree->root->color == COLOR_BLACK);
    /* constrain #3 */
    assert(tree->nil->color == COLOR_BLACK);
    /* constrain #4 #5 */
    node_validate(tree, tree->root);
    return 0;
}

int rbtree_set(struct rbtree *tree, int key, int val){
    struct rbtnode *new, *parent, *nil = tree->nil, **indirect;
    indirect = &tree->root;
    parent = *indirect;
    for (;;){
        // this field is containing "NIL"
        if ((*indirect) == nil){
            /*
                we've found an empty place, implies no corresponding key
                was found.
            */
            new = rbtnode_alloc(tree, key, val);
            new->parent = parent;
            // TODO: find a way to link parent
            *indirect = new;
            tree->count++;
            insert_fix(tree, new);
            break;
        }
        /* assured (*indirect) is not NULL */
        /* preserve which way coming and proceed */
        parent = *indirect;
        if (key < (*indirect)->key){
            /* indirect points to the field instead */
            indirect = &(*indirect)->left;
        } else if (key > (*indirect)->key){
            indirect = &(*indirect)->right;
        } else {
            /* we're pointing is what we're looking for */
            (*indirect)->val = val;
            break;
        }
    }
    return RBT_OK;
}

int rbtree_get(struct rbtree *tree, int key, int *res){
    struct rbtnode *cur, *nil = tree->nil;
    cur = tree->root;
    for (;cur != nil;){
        if (cur->key == key){
            *res = cur->val;
            break;
        }
        if (key < cur->key){
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    if (cur == nil){
        return RBT_NOELEM;
    }
    return RBT_OK;
}

int rbtree_delete(struct rbtree *tree, int key){
    struct rbtnode
        **indirect = &tree->root, **victim,
        *orphan, *hold, *nil = tree->nil;

    for (;*indirect != nil;){
        if ((*indirect)->key == key){
            /* do delete stuff */
            victim = find_precedence(indirect, nil);
            // we can abuse pointer to make even shorter/cleaner code here.
            if (victim != NULL){
                // abuse ver: *victim = (*victim)->left;
                orphan = (*victim)->left;
                break;
            }
            victim = find_successor(indirect, nil);
            if (victim != NULL){
                orphan = (*victim)->right;
                break;
            }
            /* it is a leaf node */
            orphan = nil;
            victim = indirect;
            /* bypassing:
             *  1) after loop condition check
             *  2) value copy
             */
            goto finish;
        }
        if (key < (*indirect)->key){
            indirect = &(*indirect)->left;
        } else {
            indirect = &(*indirect)->right;
        }
        /* then continue */
    }
    if (*indirect == nil){
        /* can't find the entry */
        return RBT_NOELEM;
    }
    /* we've completed the adopt, time to kill the victim */
    (*indirect)->key = (*victim)->key;
    (*indirect)->val = (*victim)->val;
finish:
    /*
     * set parent anyway (even it's `nil`), we'll use orphan as the
     * start of delete_fix.
     */
    hold = *victim;
    orphan->parent = (*victim)->parent;
    // "unlink" the node.
    *victim = orphan;
    /* if node we're deleting is black, we have to fix it */
    if (hold->color == COLOR_BLACK){
        delete_fix(tree, orphan);
    }
    free(hold);
    tree->count--;
    return RBT_OK;
}
