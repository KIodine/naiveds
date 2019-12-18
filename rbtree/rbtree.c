#include "rbtree.h"

static
struct rbtnode *node_alloc(int key, int val){
    struct rbtnode *node = NULL;
    node = calloc(1, sizeof(struct rbtnode));
    node->key = key;
    node->val = val;
    return node;
}

static
void tree_purge(struct rbtnode* start){
    if (start == NULL){
        return;
    }
    tree_purge(start->left);
    tree_purge(start->right);
    free(start);
    return;
}

/* TODO modify these functions */
static
struct rbtnode **find_precedence(struct rbtnode **start){
    /* find the rightmost node of left subtree */
    struct rbtnode **can;
    can = &(*start)->left;
    if (*can == NULL){
        return NULL;
    }
    for (;(*can)->right != NULL;){
        can = &(*can)->right;
    }
    return can;
}

static
struct rbtnode **find_successor(struct rbtnode **start){
    /* find the leftmost node of right subtree */
    struct rbtnode **can;
    can = &(*start)->right;
    if (*can == NULL){
        return NULL;
    }
    /* only modify parent if candidate exists */
    for (;(*can)->left != NULL;){
        can = &(*can)->left;
    }
    return can;
}

static
void left_rotate(struct rbtnode *node);

static
void right_rotate(struct rbtnode *node);

static
void insert_fix(struct rbtnode *node);

static
void delete_fix(struct rbtnode *node);


struct rbtree *rbtree_alloc(void){
    struct rbtree *tree = NULL;
    tree = calloc(1, sizeof(struct rbtree));
    return tree;
}

void rbtree_purge(struct rbtree *tree){
    tree_purge(tree->root);
    tree->count = 0;
    return;
}

void rbtree_free(struct rbtree *tree){
    tree_purge(tree->root);
    free(tree);
    return;
}

int rbtree_set(struct rbtree *tree, int key, int val){
    struct rbtnode *new = NULL, **indirect;
    indirect = &tree->root;
    for (;;){
        // this field is containing NULL
        if ((*indirect) == NULL){
            /*
                we've found an empty place, implies no corresponding key
                was found.
            */
            new = node_alloc(key, val);
            *indirect = new;
            tree->count++;
            break;
        }
        /* assured (*indirect) is not NULL */
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
    struct rbtnode *cur;
    cur = tree->root;
    for (;cur != NULL;){
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
    if (cur == NULL){
        return RBT_NOELEM;
    }
    return RBT_OK;
}

int rbtree_delete(struct rbtree *tree, int key){
    struct rbtnode **indirect = &tree->root, **victim, *next, *hold;

    for (;*indirect != NULL;){
        if ((*indirect)->key == key){
            /* do delete stuff */
            victim = find_precedence(indirect);
            // we can abuse pointer to make even shorter/cleaner code here.
            if (victim != NULL){
                // abuse ver: *victim = (*victim)->left;
                next = (*victim)->left;
                break;
            }
            victim = find_successor(indirect);
            if (victim != NULL){
                next = (*victim)->right;
                break;
            }
            /* it is a leaf node */
            free(*indirect);
            /* set the branch to NULL */
            *indirect = NULL;
            /* bypass the after loop condition check and victim-killing */
            goto decr_count;
        }
        if (key < (*indirect)->key){
            indirect = &(*indirect)->left;
        } else {
            indirect = &(*indirect)->right;
        }
        /* then continue */
    }
    if (*indirect == NULL){
        /* can't find the entry */
        return RBT_NOELEM;
    }
    /* we've completed the adopt, time to kill the victim */
    (*indirect)->key = (*victim)->key;
    (*indirect)->val = (*victim)->val;
    hold = *victim;
    // "unlink" the node.
    *victim = next;
    free(hold);
decr_count:
    tree->count--;
    return RBT_OK;
}
