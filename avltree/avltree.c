#include "avltree.h"


static struct avlnode *node_alloc(int key, int val);
static void node_free(struct avlnode *node);
static int node_insert(struct avlnode **node, int key, int val);
static int node_delete(struct avlnode **node, int key);
static void node_fix(struct avlnode **node);
static void node_validate(struct avlnode *node);

static inline int get_height(struct avlnode *node);
static inline void update_height(struct avlnode *node);
static inline int max(int a, int b);
static void left_rotate(struct avlnode **a);
static void right_rotate(struct avlnode **a);
static struct avlnode **node_min(struct avlnode **node);
// static void AVLize(struct avlnode **node);

 
static inline
int get_height(struct avlnode *node){
    return (node == NULL)? -1: node->height;
}

static inline
int max(int a, int b){
    return a > b? a: b;
}

static inline
void update_height(struct avlnode *node){
    int lh, rh;
    lh = get_height(node->child[CHILD_LEFT]);
    rh = get_height(node->child[CHILD_RIGHT]);
    node->height = max(lh, rh) + 1;
    return;
}

static void node_fix(struct avlnode **node){
    int lh, rh, diff, dir;
    struct avlnode **b;
    static void (*rotator[2])(struct avlnode **) = {
        left_rotate, right_rotate
    };
    lh = get_height((*node)->child[CHILD_LEFT]);
    rh = get_height((*node)->child[CHILD_RIGHT]);
    diff = abs(lh - rh);
    if (diff > 1){
        // check is LL|RR or LR|RL case
        // check is L or R
        // 0: L, 1: R
        dir = (lh > rh);
        b = &(*node)->child[!dir];
        lh = get_height((*b)->child[CHILD_LEFT]);
        rh = get_height((*b)->child[CHILD_RIGHT]);
        if ((lh > rh) != dir){
            // not the same case as parent
            // LR|RL case
            rotator[!dir](b);
        }
        rotator[dir](node);
    } else {
        // do nothing, just update.
        update_height(*node);
    }
    return;
}

static void left_rotate(struct avlnode **a){
    struct avlnode *b = (*a)->child[CHILD_RIGHT];
    (*a)->child[CHILD_RIGHT] = b->child[CHILD_LEFT];
    b->child[CHILD_LEFT] = (*a);
    (*a) = b;
    /*
        A            B
       / \          / \
      a   B   ->   A   c
         / \      / \
        b   c    a   b
    */
    update_height(b->child[CHILD_LEFT]);
    update_height(b);
    return;
}

static void right_rotate(struct avlnode **a){
    struct avlnode *b = (*a)->child[CHILD_LEFT];
    (*a)->child[CHILD_LEFT] = b->child[CHILD_RIGHT];
    b->child[CHILD_RIGHT] = (*a);
    (*a) = b;
    /*
        A            B 
       / \          / \
      B   c   ->   a   A
     / \              / \
    a   b            b   c
    */
    // the content of (*a) is now b.
    update_height(b->child[CHILD_RIGHT]);
    update_height(b);
    return;
}

static struct avlnode **node_min(struct avlnode **node){
    struct avlnode **res = node;
    for (;(*res)->child[CHILD_LEFT] != NULL;){
        res = &(*res)->child[CHILD_LEFT];
    }
    return res;
}

static
struct avlnode *node_alloc(int key, int val){
    struct avlnode *node;
    node = calloc(1, sizeof(struct avlnode));
    node->key = key;
    node->val = val;
    return node;
}

static
void node_free(struct avlnode *node){
    if (node == NULL){
        return;
    }
    node_free(node->child[CHILD_LEFT]);
    node_free(node->child[CHILD_RIGHT]);
    free(node);
    return;
}

static
int node_insert(struct avlnode **node, int key, int val){
    int ret = 0;
    struct avlnode *newnode = NULL;
    if (*node == NULL){
        newnode = node_alloc(key, val);
        *node = newnode;
    } else {
        if ((*node)->key > key){
            ret = node_insert(&(*node)->child[CHILD_LEFT], key, val);
        } else if ((*node)->key < key) {
            ret = node_insert(&(*node)->child[CHILD_RIGHT], key, val);
        } else {
            // key is occupied.
            ret = -1;
        }
    }
    if (ret == -1){
        // no insert is made, bail out.
        return ret;
    }
    node_fix(node);
    //update_height(*node);
    return 0;
}


static
int node_delete(struct avlnode **node, int key){
    int ret = 0;
    struct avlnode *tmp = NULL, **victim;
    if ((*node) == NULL){
        // cannot find target, bail out.
        ret = -1;
    } else {
        if (key < (*node)->key){
            ret = node_delete(&(*node)->child[CHILD_LEFT], key);
        } else if (key > (*node)->key){
            ret = node_delete(&(*node)->child[CHILD_RIGHT], key);
        } else {
            // find target
            // one child|no child case
            if ((*node)->child[CHILD_LEFT] == NULL){
                tmp = (*node);
                *node = (*node)->child[CHILD_RIGHT];
                free(tmp);
            } else if ((*node)->child[CHILD_RIGHT] == NULL){
                tmp = (*node);
                *node = (*node)->child[CHILD_LEFT];
                free(tmp);
            } else {
                // 2 child case
                victim = node_min(&(*node)->child[CHILD_RIGHT]);
                //victim = node_successor(node);
                (*node)->key = (*victim)->key;
                (*node)->val = (*victim)->val;
                key = (*victim)->key;
                // successor is the leftmost node of right subtree.
                ret = node_delete(&(*node)->child[CHILD_RIGHT], key);
            }
        }
    }
    // bail out or do fixup...
    if (ret == -1){
        return ret;
    }
    if (*node != NULL){
        node_fix(node);
    }
    return 0;
}

static void node_validate(struct avlnode *node){
    int lh, rh, diff;
    if (node == NULL){
        return;
    } else {
        node_validate(node->child[CHILD_LEFT]);
        node_validate(node->child[CHILD_RIGHT]);
    }
    // check basic BST rules 
    if (node->child[CHILD_LEFT] != NULL){
        assert(node->child[CHILD_LEFT]->key < node->key);
    }
    if (node->child[CHILD_RIGHT] != NULL){
        assert(node->child[CHILD_RIGHT]->key > node->key);
    }
    // check AVL tree rules
    lh = get_height(node->child[CHILD_LEFT]);
    rh = get_height(node->child[CHILD_RIGHT]);
    diff = abs(lh - rh);
    assert(diff < 2);
    return;
}

struct avltree *avl_alloc(void){
    struct avltree *tree;
    tree = calloc(1, sizeof(struct avltree));
    return tree;
}

void avl_purge(struct avltree *tree){
    node_free(tree->root);
    tree->count = 0;
    return;
}

void avl_free(struct avltree *tree){
    node_free(tree->root);
    free(tree);
    return;
}

int avl_get(struct avltree *tree, int key, int *res){
    int ret = 0;
    struct avlnode *tmp;
    tmp = tree->root;
    for (;tmp != NULL;){
        if (key > tmp->key){
            tmp = tmp->child[CHILD_RIGHT];
        } else if (key < tmp->key){
            tmp = tmp->child[CHILD_LEFT];
        } else {
            *res = tmp->val;
            break;
        }
    }
    if (tmp == NULL){
        ret = -1;
    }
    return ret;
}

int avl_insert(struct avltree *tree, int key, int val){
    int ret;
    ret = node_insert(&tree->root, key, val);
    if (ret == 0){
        tree->count++;
    }
    return ret;
}

int avl_delete(struct avltree *tree, int key){
    int ret;
    ret = node_delete(&tree->root, key);
    if (ret == 0){
        tree->count--;
    }
    return ret;
}

void avl_validate(struct avltree *tree){
    return node_validate(tree->root);
}