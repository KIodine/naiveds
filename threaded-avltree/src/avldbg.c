#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "avldbg.h"
#include "list.h"


#define NL "\n"


static void node_print(struct avlnode *node, int depth);
static int node_validate(struct avlnode *node, avl_cmp_t cmp);
static int list_validate(struct list *head, avl_cmp_t cmp);
/* TODO: Implement validation of `avl_get_min` and `avl_get_max`. */
static struct avlnode *get_tree_min(struct avltree *tree);
static struct avlnode *get_tree_max(struct avltree *tree);


void avl_print(struct avltree *tree){
    putchar('\n');
    node_print(tree->root, 0);
    return;
}

static
void node_print(struct avlnode *node, int depth){
    if (node == NULL){
        return;
    }
    node_print(node->child[CLD_R], depth + 1);
    for (int i = 0; i < depth; ++i){
        printf("    ");
    }
    printf("+-[%d]"NL, node->factor);
    node_print(node->child[CLD_L], depth + 1);
    return;
}

void avl_validate(struct avltree *tree){
    struct avlnode *m_list, *m_recur;
    int recur_count, iter_count;
    
    /* Check AVL tree attributes. */
    recur_count = node_validate(tree->root, tree->cmp);
    /* Check is list sorted. */
    iter_count = list_validate(&tree->head, tree->cmp);
    /* Check all nodes are linked properly. */
    assert(recur_count == iter_count);
    //printf("recur:%d"NL"iter: %d"NL, recur_count, iter_count);

    m_list = avl_get_min(tree);
    m_recur = get_tree_min(tree);
    assert(m_list == m_recur);
    
    m_list = avl_get_max(tree);
    m_recur = get_tree_max(tree);
    assert(m_list == m_recur);

    return;
}

static
int node_validate(struct avlnode *node, avl_cmp_t cmp){
    int res, count = 0;
    if (node == NULL){
        return 0;
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
        assert(node->child[CLD_L]->parent == node);
    }
    if (node->child[CLD_R] != NULL){
        assert(node->child[CLD_R]->parent == node);
    }
    count += node_validate(node->child[CLD_L], cmp);
    count += node_validate(node->child[CLD_R], cmp);
    return count + 1;
}

static int list_validate(struct list *head, avl_cmp_t cmp){
    struct avlnode *ncur, *nnxt;
    struct list *cur, *next;
    int count = 0;

    for (cur = head->next; cur != head;){
        next = cur->next;
        count++;
        if (next == head){
            break;
        }
        ncur = container_of(cur, struct avlnode, node);
        nnxt = container_of(next, struct avlnode, node);
        assert(cmp(nnxt, ncur) == NODE_GT);
        
        cur = next;
    }
    return count;
}

static struct avlnode *get_tree_min(struct avltree *tree){
    struct avlnode *cur = tree->root;
    for (;cur->child[CLD_L] != NULL;){
        cur = cur->child[CLD_L];
    }
    return cur;
}

static struct avlnode *get_tree_max(struct avltree *tree){
    struct avlnode *cur = tree->root;
    for (;cur->child[CLD_R] != NULL;){
        cur = cur->child[CLD_R];
    }
    return cur;
}