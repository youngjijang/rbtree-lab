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
  if (node == t->nil) return;
  post_order(t,node->left);
  post_order(t,node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if(t->root != t->nil){
    post_order(t,t->root);
  }
  free(t->nil);
  free(t);
}

/**
 * 왼쪽 회전..
 */
void left_rotate(rbtree *t, node_t *p){
  if(p->right != t->nil){ //nil사용시 안해줘도됨

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

}

/**
 * 오른쪽 회전
 */
void right_rotate(rbtree *t, node_t *p){
  if(p->left != t->nil){

    node_t *y = p->left;
    p->left = y->right;
    if(y->right != t->nil)
      y->right->parent = p;

    y->parent = p->parent;

    if(p->parent == t->nil)
      t->root = y;
    else if (p == p->parent->right)
      p->parent->right = y;
    else p->parent->left = y;

    y->right = p;
    p->parent = y;
  }

}

/**
 * 삽입후 위반 사항 처리
 */
void insert_fixed(rbtree *t, node_t *p){
  node_t* y;

  while(p->parent->color == RBTREE_RED){
    if(p->parent == p->parent->parent->left){ // 왼쪽일떄
      y = p->parent->parent->right;
      if(y->color == RBTREE_RED){//case 1
        p->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        p = p->parent->parent;
      }else{
        if(p == p->parent->right){ //case 2
          p = p->parent;
          left_rotate(t,p);
        }
        p->parent->color= RBTREE_BLACK; //case 3
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
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *p = (node_t*)malloc(sizeof(node_t));
  p->key = key;
  p->left = t->nil;
  p->right = t->nil;
  p->color = RBTREE_RED;

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
  else if(key < y->key)
    y->left = p;
  else 
    y->right = p;

  insert_fixed(t,p);
  t->root->color = RBTREE_BLACK;

  return p;
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

// 입력받은 노드기준 최소값 찾기
node_t *min(const rbtree *t,node_t* node){
  while(node->left != t->nil){
    node = node->left;
  }
  return node;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *tmp = min(t,t->root);
  return tmp;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *tmp = t->root;
  while(tmp->right != t->nil){
    tmp = tmp->right;
  }
  // printf("%d\n",tmp->key);
  return tmp;
}

//이식
void tranceplant(rbtree *t, node_t *u,node_t *v){
  if (u->parent == t->nil)
    t->root = v;
  else if(u == u->parent->left)
    u->parent->left = v;
  else u->parent->right = v;
  v->parent = u->parent;
}

void erase_fixed(rbtree *t, node_t *x){
  node_t *w;
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if(x == x->parent->left){
      w = x->parent->right;
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t,x->parent);
        w = x->parent->right;
      }
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t,w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t,x->parent);
        x = t->root;
      }
    }else{
      w = x->parent->left;
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w = x->parent->left;
      }
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t,w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t,x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}


int rbtree_erase(rbtree *t, node_t *p) {
  if(p == t->nil) return 0;
  // TODO: implement erase
  node_t *x;
  node_t *y;
  color_t origin = p->color;
  //자식이 하나 혹은 없을 경우
  if(p->left == t->nil){
    x = p->right;
    tranceplant(t,p,p->right);
  }
  else if (p->right == t->nil){
    x = p->left;
    tranceplant(t,p,p->left);
  }
  //자식이 둘인 경우 
  else{
    y = min(t,p->right); //sucessor
    origin = y->color;
    x = y->right;// 자식이 없으면 nil
    if(y->parent == p) // 바로 연결되어있을때?
      x->parent = y; 
    else {
      tranceplant(t,y,y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    tranceplant(t,p,y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if(origin == RBTREE_BLACK)
    erase_fixed(t,x);

  free(p);
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


// void inorder(rbtree *t, node_t *x)
// {
//   // printf(“색깔 : %d, 키 : %d / “, x->color, x->key);
//   if (x != t->nil)
//   {
//     inorder(t, x->left);
//     printf("색깔 : %d, 키 : %d\n", x->color, x->key);
//     inorder(t, x->right);
//   }
// }

// int main()
// {
//   rbtree *t = new_rbtree();
//   key_t entries[] = {10, 9, 8, 13, 12, 11};
//   const size_t n = sizeof(entries) / sizeof(entries[0]);
//   for (size_t i = 0; i < n; i++)
//   {
//     rbtree_insert(t, entries[i]);
//   }
//   inorder(t, t->root);
// }