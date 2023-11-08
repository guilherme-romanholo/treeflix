#include "bp_tree.h"
#include <stdbool.h>
#include <stdlib.h>

#define ORDER 3

struct node {
  int *keys;
  int num_keys;
  bool is_leaf;
  struct node **ptrs;
  struct node *parent;
  struct node *next;
};

struct bptree {
  Node *root;
};

Node *Node__create(bool is_leaf) {
  Node *new_node = malloc(sizeof(Node));

  new_node->keys = calloc(sizeof(int), ORDER);
  new_node->ptrs = calloc(sizeof(void), ORDER + 1);
  new_node->is_leaf = is_leaf;
  new_node->num_keys = 0;
  new_node->parent = NULL;
  new_node->next = NULL;

  return new_node;
}

BPTree *BPTree__create() {
  BPTree *tree = malloc(sizeof(BPTree));

  tree->root = NULL;

  return tree;
}

Node *BPTree__find_leaf(Node *root, int key) {
  int i;

  if (!root)
    return root;

  while (!root->is_leaf) {
    i = 0;

    while (i < root->num_keys) {
      if (key >= root->keys[i])
        i++;
      else
        break;
    }

    root = root->ptrs[i];
  }

  return root;
}

Node *BPTree__search(Node *root, Node **leaf_out, int key) {
  if (!root) {
    if (leaf_out)
      *leaf_out = NULL;
    return NULL;
  }

  int i;
  Node *leaf = BPTree__find_leaf(root, key);

  for (i = 0; leaf->num_keys; i++) {
    if (leaf->keys[i] == key)
      break;
  }

  if (leaf_out)
    *leaf_out = leaf;

  if (i == leaf->num_keys)
    return NULL;
  else
    return leaf->ptrs[i];
}
