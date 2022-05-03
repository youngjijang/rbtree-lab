#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  
  p->nil = (node_t*)calloc(1,sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  
  p->root = p->nil;
  return p;
}

/**
 * 후위 순회 노드 삭제
 */
void post_order(rbtree *t, node_t *node){
  if (node != t->nil) {
    post_order(t,node->left);
    post_order(t,node->right);
    free(node);
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  post_order(t,t->root);
  free(t->nil);
  free(t);
}

/**
 * 왼쪽 회전..
 */
void left_rotate(rbtree *t, node_t *p){
  if(p == NULL) return;

  node_t *y = p->right;
  p->right = y->left;
  if(y->left != t->nil)
    y->left->parent = p;

  y->parent = p->parent;

  if(p->parent == t->nil)
    t->root = y;
  else if (p == p->parent->left)
    p->parent->left = y;
  else p->parent->right = y;

  y->left = p;
  p->parent = y;

}

/**
 * 오른쪽 회전
 */
void right_rotate(rbtree *t, node_t *p){
  if(p == NULL) return;

  node_t *y = p->left;
  p->left = y->right;
  if(y->right != t->nil)
    y->right->parent = p;

  y->parent = p->parent;

  if(p->parent == t->nil)
    t->root = y;
  else if (p == p->parent->left)
    p->parent->right = y;
  else p->parent->left = y;

  y->right = p;
  p->parent = y;

}

/**
 * 삽입후 위반 사항 처리
 */
void inset_fixed(rbtree *t, node_t *p){
  node_t* y = t->nil;

  while(p->parent->color == RBTREE_RED){
    if(p->parent == p->parent->parent->left){ // 왼쪽일떄
      y = p->parent->parent->right;
      if(y->color == RBTREE_RED){
        p->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        p = p->parent->parent;
      }else{
        if(p == p->parent->right){
          p = p->parent;
          left_rotate(t,p);
        }
        p->parent->color= RBTREE_BLACK;
        p->parent->parent->color =RBTREE_RED;
        right_rotate(t,p->parent->parent);
      }
    }else{ //오른쪽일때
      y = p->parent->parent->left;
      if(y->color == RBTREE_RED){
        p->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        p = p->parent->parent;
      }else{
        if(p == p->parent->left){
          p = p->parent;
          right_rotate(t,p);
        }
        p->parent->color= RBTREE_BLACK;
        p->parent->parent->color =RBTREE_RED;
        left_rotate(t,p->parent->parent);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *p = (node_t*)malloc(sizeof(node_t));
  p->key = key;
  p->left = t->nil;
  p->right = t->nil;

  node_t *y = t->nil;
  node_t *x = t->root;

  while (x != t->nil){
    y = x;
    if(key < x->key)
      x = x->left;
    else x = x->right;
  }
  p->parent = y; //

  if(y == t->nil)
    t->root = p; // 하나도 없을 경우
  else if(p->key < y->key)
    y->left = p;
  else y->right = p;

  inset_fixed(t,p);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *tmp = t->root;
  while(tmp != t->nil){
    if(tmp->key == key)
      return tmp;
    if(tmp->key > key)
      tmp = tmp->left;
    else
      tmp = tmp->right;
  }
  return NULL;
}


node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *tmp = t->root;
  while(tmp != t->nil){
    tmp = tmp->left;
  }
  return tmp;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *tmp = t->root;
  while(tmp != t->nil){
    tmp = tmp->right;
  }
  return tmp;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int inorder_tree_work(const rbtree *t, const node_t *x, key_t *arr, int i) {
  if (x != t->nil) {
    i = inorder_tree_work(t, x->left, arr, i);
    
    arr[i] = x->key;
    // printf("%d번 %d색",x->key,x->color);
    i = i+1;
    i = inorder_tree_work(t, x->right, arr, i);
  }
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  
  inorder_tree_work(t, t->root, arr, 0);
  return 0;
}


void inorder(rbtree *t, node_t *x)
{
  // printf(“색깔 : %d, 키 : %d / “, x->color, x->key);
  if (x != t->nil)
  {
    inorder(t, x->left);
    printf("색깔 : %d, 키 : %d\n", x->color, x->key);
    inorder(t, x->right);
  }
}

int main()
{
  rbtree *t = new_rbtree();
  key_t entries[] = {2, 3, 4, 5,1,3,6 ,8};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  for (size_t i = 0; i < n; i++)
  {
    rbtree_insert(t, entries[i]);
  }
  inorder(t, t->root);
}