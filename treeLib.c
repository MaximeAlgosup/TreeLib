#include "treeLib.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void tree_create(struct tree *self) {
  self->root = NULL;
}

struct tree_node *tree_del_min(struct tree_node *node, struct tree_node **min){
  if(node->left == NULL){
    struct tree_node *right =node->right;
    node->right = NULL;
    *min = node;
    return right;
  }
  node->left = tree_del_min(node->left,min);
  return node;
}

struct tree_node *tree_destroy_partial(struct tree_node *node){
  struct tree_node *left = node->left;
  struct tree_node *right = node->right;
  free(node);
  node =NULL;
  if((left == NULL)&&(right ==NULL)){
    return NULL;
  }
  if(left == NULL){
    return right;
  }
  if(right == NULL){
    return left;
  }
  right = tree_del_min(right,&node);
  node->left = left;
  node->right = right;
  return node;
}

void tree_destroy(struct tree *self) {
  struct tree_node *node;
  size_t size = tree_size(self);
  for(size_t i=0; i<size; ++i){
    node= self->root;
    self->root = tree_destroy_partial(node);
  }
}

bool tree_node_search(const struct tree_node *self, int value){
  if (self == NULL) {
    return false;
  }
  if (value < self->data) {
    return tree_node_search(self->left, value);
  }
  if (value > self->data) {
    return tree_node_search(self->right, value);
  }
  return true;
}

bool tree_contains(const struct tree *self, int value) {
  return tree_node_search(self->root,value);
}

struct tree_node *add_node(struct tree_node *node, int value){
    if (node == NULL){
      struct tree_node *new= malloc(sizeof(struct tree_node));
      new->left = new->right = NULL;
      new->data = value;
      return new;
    }
    if (value < node->data){
      node->left = add_node(node->left, value);
    }
    else if (value > node->data){
      node->right = add_node(node->right, value);
    }
    return node;
}

bool tree_insert(struct tree *self, int value) {
  if(tree_contains(self,value)) return false;
  struct tree_node *curr = self->root;
  self->root = add_node(curr,value);
  return true;
}

struct tree_node *tree_remove_partial(struct tree_node *node, int value){
  if(node == NULL){
    return NULL;
  }
  if(value < node->data){
    node->left = tree_remove_partial(node->left, value);
    return node;
  }
  if(value > node->data){
    node->right = tree_remove_partial(node->right, value);
    return node;
  }
  return tree_destroy_partial(node);
}

bool tree_remove(struct tree *self, int value) {
  if((tree_empty(self))||(!tree_contains(self,value))){
    return false;
  }
  struct tree_node *node = self->root;
  self->root = tree_remove_partial(node,value);
  return true;
}

bool tree_empty(const struct tree *self) {
  return (self->root == NULL);
}

size_t tree_node_nb(const struct tree_node *node){
  if(node == NULL){
    return 0;
  }
  return (tree_node_nb(node->left)+1+tree_node_nb(node->right));
}

size_t tree_size(const struct tree *self) {
  return tree_node_nb(self->root);
}

size_t tree_node_height(const struct tree_node *node){
  size_t  sizeL = 0;
  size_t sizeR = 0;
  if(node->left != NULL){
    sizeL = tree_node_nb(node->left);
  }
  if(node->right != NULL){
    sizeR = tree_node_nb(node->right);
  }
  return (sizeL>=sizeR) ? (1+sizeL):(1+sizeR);
}

size_t tree_height(const struct tree *self) {
  if(tree_empty(self)) return 0;
  else return tree_node_height(self->root);
}

void pre_order_partial(struct tree_node *node, tree_func_t func, void *user_data){
  func(node->data,user_data);
  if(node->left != NULL){
    pre_order_partial(node->left,func,user_data);
  }
  if(node->right != NULL){
    pre_order_partial(node->right,func,user_data);
  }
}

void tree_walk_pre_order(const struct tree *self, tree_func_t func, void *user_data){
  if(tree_empty(self))return;
  else{
    pre_order_partial(self->root,func,user_data);
  }
}

void in_order_partial(struct tree_node *node, tree_func_t func, void *user_data){
  if(node->left != NULL){
    in_order_partial(node->left,func,user_data);
  }
  func(node->data,user_data);
  if(node->right != NULL){
    in_order_partial(node->right,func,user_data);
  }
}

void tree_walk_in_order(const struct tree *self, tree_func_t func, void *user_data) {
  if(tree_empty(self))return;
  else{
    in_order_partial(self->root,func,user_data);
  }
}

void post_order_partial(struct tree_node *node, tree_func_t func, void *user_data){
  if(node->left != NULL){
    post_order_partial(node->left,func,user_data);
  }
  if(node->right != NULL){
    post_order_partial(node->right,func,user_data);
  }
  func(node->data,user_data);
}

void tree_walk_post_order(const struct tree *self, tree_func_t func, void *user_data) {
  if(tree_empty(self))return;
  else{
    post_order_partial(self->root,func,user_data);
  }
}
