#include "rbtree.h"


#ifdef __GNUC__
    #define FORCE_INLINE __attribute__((always_inline))
#elif
    #define FORCE_INLINE
#endif


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
        // tree->root = y
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

static
void insert_fix(struct rbtree *tree, struct rbtnode *node);

static
void delete_fix(struct rbtree *tree, struct rbtnode *node);


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
    /* maybe place `delete_fix` here */
    free(hold);
    tree->count--;
    return RBT_OK;
}
