#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

void postOrder(rbtree *t, node_t *cur)
{
    if (cur == t->nil)
    {
        // printf("nil ");
        return;
    }

    postOrder(t, cur->left);
    postOrder(t, cur->right);
    printf("%d ", cur->key);
}

void rbtree_to_print(rbtree *t, node_t *cur)
{
    if (cur == t->nil)
    {
        return;
    }
 
    node_t *current = cur;
    while (current != t->nil)
    {
        printf("%d\n", current->key);
        current = current->left;
    }

    while (cur != t->nil)
    {
        node_t *temp = cur->right;
        while (temp != t->nil)
        {
            printf("%d\n", temp->key);
            temp = temp->left;
        }
        cur = cur->left;
    }
}

int main(int argc, char *argv[])
{
    rbtree *t = new_rbtree();

    for (int i = 1; i <= 15; i++)
    {
        rbtree_insert(t, i);
        postOrder(t, t->root);
    }

    rbtree_to_print(t, t->root);
    return 0;
}