#include "bstree.h"

static
BSTNode *node_alloc(int key, int val){
    BSTNode *node = NULL;
    node = calloc(1, sizeof(BSTNode));
    node->key = key;
    node->val = val;
    return node;
}

static
void tree_purge(BSTNode* start){
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
BSTNode *find_precedence(BSTNode *start, BSTNode **parent){
    /* find the rightmost node of left subtree */
    BSTNode *can;
    can = start->left;
    if (can == NULL){
        return NULL;
    }
    *parent = start;
    for (;can->right != NULL;){
        *parent = can;
        can = can->right;
    }
    return can;
}

static
BSTNode *find_successor(BSTNode *start, BSTNode **parent){
    /* find the leftmost node of right subtree */
    BSTNode* can;
    can = start->right;
    if (can == NULL){
        return NULL;
    }
    /* only modify parent if candidate exists */
    *parent = start;
    for (;can->left != NULL;){
        *parent = can;
        can = can->left;
    }
    return can;
}



BSTree *bstree_alloc(void){
    BSTree *tree = NULL;
    tree = calloc(1, sizeof(BSTree));
    return tree;
}

void bstree_free(BSTree *tree){
    tree_purge(tree->root);
    free(tree);
    return;
}

int bstree_set(BSTree *tree, int key, int val){
    BSTNode *new = NULL, *cur, **indirect;
    /*
        `indirect` holds the pointer to either `left` or `right`
        of `BSTNode`.
        the benefit of using `indirect` is that we can "directly" modify
        the field no matter what the form of the structure holding it,
        as long as all indirect pointer follow the same rule.
    */
/*  (preserve for study purpose)
    if (tree->root == NULL){
        new = node_alloc(key, val);
        tree->root = new;
        tree->count++;
        return BST_OK;
    }
    cur = tree->root;
    for (;;){
        if (key == cur->key){
            cur->val = val;
            break;
        }
        // decide which "way" we're modifying
        if (key < cur->key){
            indirect = &cur->left;
        } else {
            indirect = &cur->right;
        }
        if (*indirect != NULL){
            // if still branch to compare
            cur = *indirect;
        } else {
            // found a space
            new = node_alloc(key, val);
            // link the branch
            *indirect = new;
            tree->count++;
            break;
        }
    }
*/
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
    return BST_OK;
}

int bstree_get(BSTree *tree, int key, int *res){
    BSTNode *cur;
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
        return BST_NOELEM;
    }
    return BST_OK;
}

/*  once find the correct entry:

    find precedence and its parent
    if precedence is not NULL
        copy key and val from precedence
        parend "adopt" left child of precedence
        free precedence
        set the parent's right to NULL
    else if successor is not NULL
        copy key and val from precedence
        parend "adopt" right child of successor
        free successor
        set the parent's left to NULL
    else
        (node is leaf node)
        free content of indirect
        (essentially `free(node->left)...`)
        set content of indirect to NULL
*/
/*
    Inspired by "Linus' good taste of coding".
    the use of `indirect` eliminates the need to check
    whether a node is the root or not, since it
    **"directly" points to the address of the field**
    (which its type is "pointer to struct", indirect is
    "pointer of pointer to struct").
    
    if we know where the thing(pointer to struct) is, we can use it and
    even modify the field holds it.

    Or was it?
*/
int bstree_delete(BSTree *tree, int key){
    BSTNode **indirect = &tree->root, *parent, *victim;

    for (;*indirect != NULL;){
        if ((*indirect)->key == key){
            /* do delete stuff */
            victim = find_precedence(*indirect, &parent);
            // we can abuse pointer to make even shorter/cleaner code here.
            if (victim != NULL){
                /* the "adopt", now victim is alone */
                if (parent != *indirect){
                    parent->right = victim->left;
                } else {
                    parent->left = victim->left;
                }
                break;
            }
            victim = find_successor(*indirect, &parent);
            if (victim != NULL){
                if (parent != *indirect){
                    parent->left = victim->right;
                } else {
                    parent->right = victim->right;
                }
                break;
            }
            /* it is a leaf node */
            free(*indirect);
            /* set the branch to NULL */
            *indirect = NULL;
//          break;
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
        return BST_NOELEM;
    }
    /* we've completed the adopt, time to kill the victim */
    (*indirect)->key = victim->key;
    (*indirect)->val = victim->val;
    free(victim);
decr_count:
    tree->count--;
    return BST_OK;
}
