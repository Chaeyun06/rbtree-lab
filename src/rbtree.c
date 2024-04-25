#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *sentinel = (node_t *)calloc(1, sizeof(node_t));
  p->nil = sentinel;
  p->root = p->nil;
  sentinel->color = RBTREE_BLACK;

  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  if (t == NULL || t->root == t->nil)
  {
    return;
  }
  delete_node(t, t->root);
  free(t->nil);
  free(t);
  t->root = NULL;
}

void delete_node(rbtree *t, node_t *cur)
{
  if (cur == t->nil)
  {
    return;
  }
  delete_node(t, cur->left);
  delete_node(t, cur->right);

  if (cur->parent != t->nil)
  {
    if (cur->parent->left == cur)
    {
      cur->parent->left = t->nil;
    }
    else
    {
      cur->parent->right = t->nil;
    }
  }

  free(cur);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *y; // 삽입할 z의 부모 노드
  y = t->nil;

  node_t *x; // 삽입할 공간 탐색 노드
  x = t->root;

  node_t *z = (node_t *)calloc(1, sizeof(node_t)); // 삽입할 노드
  z->key = key;

  while (x != t->nil)
  {
    y = x;
    if (z->key < x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }

  z->parent = y;

  if (y == t->nil)
  {
    t->root = z;
  }
  else if (z->key < y->key)
  {
    y->left = z;
  }
  else
  {
    y->right = z;
  }

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  rb_insert_fixup(t, z);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *cur = t->root;

  while (cur != t->nil)
  {
    if (cur->key == key)
    {
      return cur;
    }

    if (cur->key > key)
    {
      cur = cur->left;
    }
    else
    {
      cur = cur->right;
    }
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  if (t->root == t->nil)
    return t->root;

  node_t *search_min = t->root->left;

  while (search_min->left != t->nil)
  {
    search_min = search_min->left;
  }

  return search_min;
}

node_t *rbtree_max(const rbtree *t)
{
  if (t->root == t->nil)
    return t->root;

  node_t *search_max = t->root->right;

  while (search_max->right != t->nil)
  {
    search_max = search_max->right;
  }

  return search_max;
}

int rbtree_erase(rbtree *t, node_t *z)
{
  node_t *y = z;
  color_t y_origin_color = y->color;
  node_t *x;

  // successor 찾기
  if (z->left == t->nil)
  {
    x = z->right;
    transplant(t, z, z->right);
  }
  else if (z->right == t->nil)
  {
    x = z->left;
    transplant(t, z, z->left);
  }
  else
  {
    y = minimum_node(t, z->right); // 후임자
    y_origin_color = y->color;
    x = y->right;
    if (y->parent == z)
    { // 후임자가 삭제할 노드 바로 밑이면
      x->parent = y;
    }
    else
    {
      transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  free(z);

  if (y_origin_color == RBTREE_BLACK)
  {
    delete_fixup(t, x);
  }

  return 0;
}

void delete_fixup(rbtree *t, node_t *x)
{
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else
    {
      // x == x->parent->left 이 조건은 이미 위의 if문에서 검사되므로 생략
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

node_t *minimum_node(rbtree *t, node_t *sub_tree)
{
  node_t *cur = sub_tree;
  while (cur->left != t->nil)
  {
    cur = cur->left;
  }
  return cur;
}

void transplant(rbtree *t, node_t *z, node_t *x)
{
  if (z->parent == t->nil)
  {
    t->root = x;
  }
  else if (z == z->parent->left)
  {
    z->parent->left = x;
  }
  else
  {
    z->parent->right = x;
  }
  x->parent = z->parent;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  inorder_search(t->nil, t->root, 0, arr, n);

  return 0;
}

int inorder_search(node_t *sentinel, node_t *cur, int idx, key_t *arr, const size_t n)
{
  // 재귀 탈출 조건
  if (cur == sentinel || idx >= n)
    return idx;

  idx = inorder_search(sentinel, cur->left, idx, arr, n);
  arr[idx++] = cur->key;
  idx = inorder_search(sentinel, cur->right, idx, arr, n);
  return idx;
}

void rb_insert_fixup(rbtree *t, node_t *z)
{
  node_t *y;

  while (z->parent->color == RBTREE_RED)
  {
    if (z->parent == z->parent->parent->left)
    { // parent가 왼쪽
      y = z->parent->parent->right;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->right)
        {
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else if (z->parent == z->parent->parent->right)
    { // parent가 오른쪽
      y = z->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
  return;
}
// rbtree: *root, *nil
// node_t: color, key, *parent, *left, *right
void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;

  if (y->left != t->nil)
  {
    y->left->parent = x;
  }

  y->parent = x->parent;

  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x->parent->left == x)
  { // x가 왼쪽 자식이면
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}
void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;

  x->left = y->right;

  if (y->right != t->nil)
  {
    y->right->parent = x;
  }

  y->parent = x->parent;

  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x->parent->right == x)
  {
    x->parent->right = y;
  }
  else
  {
    x->parent->left = y;
  }

  y->right = x;
  x->parent = y;
}

void printTree(rbtree *t, node_t *cur, int level, int isLeft)
{
  if (cur == t->nil)
  {
    return;
  }

  // 오른쪽 자식 노드 출력
  printTree(t, cur->right, level + 1, 0);

  // 현재 노드 출력
  for (int i = 0; i < level - 1; i++)
  {
    printf("    ");
  }
  if (level > 0)
  {
    printf(isLeft ? " \\_ " : " /⎺ "); // 왼쪽 자식일 경우 "\\" 출력, 오른쪽 자식일 경우 "/" 출력
  }
  if (cur->color == RBTREE_RED)
  {
    printf("\x1b[31m%d\x1b[0m\n", cur->key);
  }
  else
  {
    printf("%d\n", cur->key);
  }

  // 왼쪽 자식 노드 출력
  printTree(t, cur->left, level + 1, 1);
}