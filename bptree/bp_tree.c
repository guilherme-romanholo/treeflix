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

/* ACESSO AO ARQUIVO */

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

/* UTILIDADES */

void debug() {
  FILE *fp = fopen(TREE_FILENAME, "rb");
  Node *node = malloc(sizeof(Node));

  while (fread(node, sizeof(Node), 1, fp) == 1)
    Node__print(node);

  fclose(fp);
  free(node);
}

void Node__print(Node *node) {
  printf("--------------------\n");
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

// DEPRECIADO, utilizar o split agora
void Node__copy_keys_and_data(Node *dest, Node *ori, int s, int e) {
  for (int i = s, j = 0; i < e; i++, j++) {
    strcpy(dest->keys[j], ori->keys[i]);
    strcpy(ori->keys[i], "-----");

    dest->data_rrn[j] = ori->data_rrn[i];
    ori->data_rrn[i] = -1;

    dest->num_keys++;
    ori->num_keys--;
  }
}

void Node__split(Node *dest, Node *ori) {
  int mid = ((int)ceil((float)ori->num_keys / 2)) - 1;
  int children_size = Node__children_size(ori);
  int num_keys = ori->num_keys;

  for (int i = mid + 1, j = 0; i < num_keys; i++, j++) {
    strcpy(dest->keys[j], ori->keys[i]);
    strcpy(ori->keys[i], "-----");

    dest->data_rrn[j] = ori->data_rrn[i];
    ori->data_rrn[i] = -1;

    dest->num_keys++;
    ori->num_keys--;
  }

  for (int i = mid + 1, j = 0; i < children_size; i++, j--) {
    dest->children[j] = ori->children[i];
    ori->children[i] = -1;
  }
}

/* ROOT */

void BPTree__update_root(int root_rrn) {
  FILE *fp = fopen(ROOT_FILENAME, "r+");

  fprintf(fp, "{Root_rrn: %d}\n", root_rrn);

  fclose(fp);
}

Node *BPTree__get_root() {
  FILE *fp = fopen(ROOT_FILENAME, "r");
  int root_rrn;

  if (fp == NULL)
    return NULL;

  fscanf(fp, "{Root_rrn: %d}\n", &root_rrn);

  fclose(fp);

  return Node__read(root_rrn);
}

/* CONSTRUTURES */

BPTree *BPTree__init() {
  BPTree *tree = malloc(sizeof(BPTree));
  Node *root = BPTree__get_root();

  tree->get_root = BPTree__get_root;
  tree->update_root = BPTree__update_root;
  tree->insert = Node__insert;

  if (root == NULL) {
    FILE *fp = fopen(ROOT_FILENAME, "w");
    fprintf(fp, "{Root_rrn: %d}\n", 0);
    fclose(fp);

    root = Node__create(1);
  }

  free(root);

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

  Node__append(node);

  return node;
}

/* BUSCA DE UM NÓ */

Node *Node__search(char *key) {
  Node *cur = BPTree__get_root();

  while (cur->is_leaf == 0) {
    Node *tmp = cur;
    int num_keys = cur->num_keys;

    for (int i = 0; i < num_keys; i++) {

      if (!strcmp(key, cur->keys[i])) {
        cur = Node__read(cur->children[i + 1]);
        break;
      } else if (strcmp(key, cur->keys[i]) < 0) {
        cur = Node__read(cur->children[i]);
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

/* INSERÇÃO DE UM NÓ */

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
        leaf->data_rrn[i + 1] = data_rrn;
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

void Node__insert_in_parent(Node *old_node, Node *new_node,
                            char *promoted_key) {
  Node *root;

  if ((root = BPTree__get_root())->rrn == old_node->rrn) {
    Node *new_root = Node__create(0);

    strcpy(new_root->keys[0], promoted_key);

    new_root->children[0] = old_node->rrn;
    new_root->children[1] = new_node->rrn;

    BPTree__update_root(new_root->rrn);
    root = BPTree__get_root();

    old_node->parent = root->rrn;
    new_node->parent = root->rrn;

    new_root->num_keys++;

    Node__rewrite(old_node, old_node->rrn);
    Node__rewrite(new_node, new_node->rrn);
    Node__rewrite(new_root, new_root->rrn);

    return;
  }

  Node *parent_node = Node__read(old_node->parent);
  int children_size = Node__children_size(parent_node);

  for (int i = 0; i < children_size; i++) {
    if (parent_node->children[i] == old_node->rrn) {
      // Achamos a posição
      int pos = 0;
      for (int j = 0; j < parent_node->num_keys; j++) {
        if (strcmp(promoted_key, parent_node->keys[j]) > 0) {
          pos++;
        }
      }
      // Fazemos um shift nas chaves
      for (int j = parent_node->num_keys; j > pos; j--) {
        strcpy(parent_node->keys[j], parent_node->keys[j - 1]);
      }
      strcpy(parent_node->keys[pos], promoted_key);

      // Fazemos um shift nos filhos
      for (int j = children_size; j > (pos + 1); j--) {
        parent_node->children[j] = parent_node->children[j - 1];
      }
      parent_node->children[(pos + 1)] = new_node->rrn;

      parent_node->num_keys++;

      Node__rewrite(parent_node, parent_node->rrn);
    }

    // UNICO PROBLEMA É NOS PAIS E FILHOS DA DIREITA
    if (parent_node->num_keys == ORDER) {
      Node *parent_dash = Node__create(0);

      parent_dash->parent = parent_node->parent;
      Node__split(parent_dash, parent_node);

      int mid = ((int)ceil((float)parent_dash->num_keys / 2)) - 1;
      strcpy(promoted_key, parent_dash->keys[mid]);

      int pn_child = Node__children_size(parent_node);
      for (int j = 0; j < pn_child; j++) {
        Node *n = Node__read(parent_node->children[j]);
        n->parent = parent_node->rrn;
        Node__rewrite(n, n->rrn);
        free(n);
      }

      int pd_child = Node__children_size(parent_dash);
      for (int j = 0; j < pd_child; j++) {
        Node *n = Node__read(parent_dash->children[j]);
        n->parent = parent_dash->rrn;
        Node__rewrite(n, n->rrn);
        free(n);
      }

      Node__rewrite(parent_node, parent_node->rrn);
      Node__rewrite(parent_dash, parent_dash->rrn);

      Node__insert_in_parent(parent_node, parent_dash, promoted_key);
    }
  }
}

void Node__insert(char *key, int data_rrn) {
  Node *old_node = Node__search(key);
  Node__insert_at_leaf(old_node, key, data_rrn);

  if (old_node->num_keys == ORDER) {
    Node *new_node = Node__create(1);

    new_node->parent = old_node->parent;
    new_node->next_node = old_node->next_node;
    old_node->next_node = new_node->rrn;

    Node__split(new_node, old_node);

    Node__rewrite(new_node, new_node->rrn);
    Node__rewrite(old_node, old_node->rrn);

    Node__insert_in_parent(old_node, new_node, new_node->keys[0]);
  }
}
