#include "bp_tree.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ORDER 4

const char *TREE_FILENAME = "ibtree.idx";
const char *ROOT_FILENAME = "root.dat";

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

BPTree *BPTree__init() {
  FILE *fp = fopen(ROOT_FILENAME, "r");

  BPTree *tree = malloc(sizeof(BPTree));
  tree->insert = Node__insert;

  if (fp == NULL) {
    tree->root = Node__create(1);

    Node__append(tree->root);

    fp = fopen(ROOT_FILENAME, "w");
    fprintf(fp, "{Root_rrn: %d}\n", 0);
  } else {
    int root_rrn;

    fp = fopen(ROOT_FILENAME, "r");
    fscanf(fp, "{Root_rrb: %d}\n", &root_rrn);
    tree->root = Node__read(root_rrn);
  }

  fclose(fp);

  return tree;
}

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

void Node__append(Node *node) {
  FILE *fp = fopen(TREE_FILENAME, "ab");

  node->rrn = ftell(fp) / sizeof(Node);

  fwrite(node, sizeof(Node), 1, fp);

  fclose(fp);
}

void Node__rewrite(Node *node, int rrn) {
  FILE *fp = fopen(TREE_FILENAME, "rb+");

  fseek(fp, rrn * sizeof(Node), SEEK_SET);

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

void Node__print(Node *node) {
  printf("RRN: %d\n", node->rrn);
  printf("Parent: %d\n", node->parent);
  printf("Num keys: %d\n", node->num_keys);
  printf("Keys: ");
  for (int i = 0; i < ORDER; i++)
    printf("%s ", node->keys[i]);

  printf("\nData RRN: ");
  for (int i = 0; i < ORDER; i++)
    printf("%d ", node->data_rrn[i]);

  printf("\nChildren RRN: ");
  for (int i = 0; i < ORDER + 1; i++)
    printf("%d ", node->children[i]);

  printf("\nLeaf: %d\n", node->is_leaf);
  printf("Next node: %d\n", node->next_node);
}

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

// Não esquecer de salvar os valores no arquivo
void Node__insert_at_leaf(Node *leaf, char *key, int data_rrn) {
  if (leaf->num_keys != 0) {

    for (int i = 0; i < leaf->num_keys; i++) {

      if (!strcmp(key, leaf->keys[i])) {
        break;
      } else if (strcmp(key, leaf->keys[i]) < 0) {

        for (int j = leaf->num_keys; j > i; j--) {
          strcpy(leaf->keys[j], leaf->keys[j - 1]);
          leaf->data_rrn[j] = leaf->data_rrn[j - 1];
        }

        strcpy(leaf->keys[i], key);
        leaf->num_keys++;
        leaf->data_rrn[i] = data_rrn;

        break;
      } else if (i + 1 == leaf->num_keys) {

        strcpy(leaf->keys[i + 1], key);
        leaf->num_keys++;
        break;
      }
    }
  } else {
    strcpy(leaf->keys[0], key);
    leaf->data_rrn[0] = data_rrn;
    leaf->num_keys++;
  }

  Node__rewrite(leaf, leaf->rrn);
}

void Node__insert(Node *root, char *key, int data_rrn) {
  Node *old = Node__search(root, key);
  Node__insert_at_leaf(old, key, data_rrn);

  // if (old->num_keys == ORDER) {
  //   Node *new_node = Node__create(1);
  //
  //   new_node->parent = old->parent;
  //
  //   int mid = ((int)ceil(old->num_keys / 2)) - 1;
  //
  //   new_node->keys
  // }
}
