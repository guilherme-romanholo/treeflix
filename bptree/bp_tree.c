#include "bp_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ORDER 3

const char *TREE_FILENAME = "ibtree.idx";

struct node {
  int rrn;
  int is_leaf;
  char keys[ORDER][6];
  int data_rrn[ORDER];
  int children[ORDER + 1];
  int num_keys;
  int parent;
  int next_node;
};

Node *Node__create(int is_leaf) {
  Node *node = malloc(sizeof(Node));

  node->rrn = -1;
  node->is_leaf = is_leaf;
  node->num_keys = 0;
  node->parent = -1;
  node->next_node = -1;

  for (int i = 0; i < ORDER; i++) {
    strcpy(node->keys[i], "-----");
    node->data_rrn[i] = -1;
    node->children[i] = -1;
  }
  node->children[ORDER] = -1;

  return node;
}

void debug() {
  FILE *fp = fopen(TREE_FILENAME, "rb");
  Node *node = malloc(sizeof(Node));

  while (fread(node, sizeof(Node), 1, fp) == 1)
    Node__print(node);

  fclose(fp);
  free(node);
}

void Node__write(Node *node) {
  FILE *fp = fopen(TREE_FILENAME, "ab");

  node->rrn = ftell(fp) / sizeof(Node);

  fwrite(node, sizeof(Node), 1, fp);

  fclose(fp);
}

Node *Node__read(int rrn) {
  FILE *fp = fopen(TREE_FILENAME, "rb");
  Node *node = malloc(sizeof(Node));

  fseek(fp, rrn * sizeof(Node), SEEK_SET);

  fread(node, sizeof(Node), 1, fp);

  fclose(fp);

  return node;
}

void Node__print(Node *node) { printf("RRN: %d\n", node->rrn); }

int Node__children_size(Node *node) {
  int size = 0;

  for (int i = 0; i < ORDER + 1; i++) {
    if (node->children[i] != -1)
      size++;
  }

  return size;
}

Node *Node__search(Node *root, char *key) {
  Node *cur = root;

  while (cur->is_leaf == 0) {
    Node *tmp = cur;

    for (int i = 0; i < cur->num_keys; i++) {

      if (!strcmp(key, cur->keys[i])) {
        cur = Node__read(cur->children[i + 1]);
        break;
      } else if (strcmp(key, cur->keys[i]) < 0) {
        cur = Node__read(cur->children[i + 1]);
        break;
      } else if (i + 1 == Node__children_size(cur)) {
        cur = Node__read(cur->children[i + 1]);
        break;
      }
    }

    free(tmp);
  }

  return cur;
}
