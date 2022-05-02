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
void left_rotate(rbtree *t, const key_t key){
  node_t *x = rbtree_find(t,key);
  if(x == NULL) return;

  node_t *y = x->right;
  x->right = y->left;
  if(y->left != t->nil)
    y->left->parent = x;

  y->parent = x->parent;

  if(x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else x->parent->right = y;

  y->left = x;
  x->parent = y;

}

/**
 * 오른쪽 회전
 */
void right_rotate(rbtree *t, const key_t key){
  node_t *x = rbtree_find(t,key);
  if(x == NULL) return;

  node_t *y = x->left;
  x->left = y->right;
  if(y->right != t->nil)
    y->right->parent = x;

  y->parent = x->parent;

  if(x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->right = y;
  else x->parent->left = y;

  y->right = x;
  x->parent = y;

}

/**
 * 삽입후 위반 사항 처리
 */
void inset_fixed(rbtree *t, node_t *p, node_t* x,node_t* y){
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

  inset_fixed(t,p,x,y);

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
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
