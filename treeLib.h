#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct tree_node {
  int data;
  struct tree_node *left;
  struct tree_node *right;
};

struct tree {
  struct tree_node *root;
};

/*
 * Create an empty tree
 */
void tree_create(struct tree *self);

/*
 * Create a tree
 */
void tree_destroy(struct tree *self);

/*
 * Tell if the tree is empty
 */
bool tree_empty(const struct tree *self);

/*
 * Get the size of the tree
 */
size_t tree_size(const struct tree *self);

/*
 * Get the height of the tree
 */
size_t tree_height(const struct tree *self);

/*
 * Tell if a value is in the tree
 */
bool tree_contains(const struct tree *self, int value);

/*
 * Insert a value in the tree and return false if the value was already present
 */
bool tree_insert(struct tree *self, int value);

/*
 * Remove a value from the tree and return false if the value was not present
 */
bool tree_remove(struct tree *self, int value);

/*
 * A function type that takes an int and a pointer and returns void
 */
typedef void (*tree_func_t)(int value, void *user_data);

/*
 * Walk in the tree in pre order and call the function with user_data as a second argument
 */
void tree_walk_pre_order(const struct tree *self, tree_func_t func, void *user_data);

/*
 * Walk in the tree in in order and call the function with user_data as a second argument
 */
void tree_walk_in_order(const struct tree *self, tree_func_t func, void *user_data);

/*
 * Walk in the tree in post order and call the function with user_data as a second argument
 */
void tree_walk_post_order(const struct tree *self, tree_func_t func, void *user_data);


#ifdef __cplusplus
}
#endif

#endif // CONTAINERS_H
