# ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;
typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
void printTree(rbtree *t, node_t *cur, int level, int isLeft);
void right_rotate(rbtree *t, node_t *z);
void left_rotate(rbtree *t, node_t *z);
void rb_insert_fixup(rbtree *t, node_t *z);
int rbtree_erase(rbtree *, node_t *);
void delete_fixup(rbtree *t, node_t *target);
int inorder_search(node_t *sentinel, node_t *cur, int idx, key_t *arr, const size_t n);
node_t *minimum_node(rbtree *t, node_t *sub_tree);
void transplant(rbtree *t, node_t *z, node_t *x);
void delete_node(rbtree *t, node_t *cur);
void delete_rbtree(rbtree *t);
int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif  // _RBTREE_H_
