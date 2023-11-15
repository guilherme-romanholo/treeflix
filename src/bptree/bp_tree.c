#include "../../include/bp_tree.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *ROOT_FILENAME = "root.dat";

/* CONSTRUCTOR */

BPTree *BPTree__init() {
  BPTree *tree = malloc(sizeof(BPTree));

  tree->root = BPTree__read_root();
  tree->insert = Node__insert;
  tree->search = Node__search;
  tree->update_root = BPTree__update_root;

  if (tree->root == NULL) {
    FILE *fp = fopen(ROOT_FILENAME, "w");
    fprintf(fp, "{Root_rrn: %d}\n", 0);
    fclose(fp);

    tree->root = Node__create(1);
  }

  return tree;
}

/* DESTRUCTOR */

void BPTree__free(BPTree *tree) {
  free(tree->root);
  free(tree);
}

/* ROOT MANIPULATION */

Node *BPTree__read_root() {
  FILE *fp = fopen(ROOT_FILENAME, "r");
  int root_rrn;

  if (fp == NULL)
    return NULL;

  fscanf(fp, "{Root_rrn: %d}\n", &root_rrn);

  fclose(fp);

  return Node__read(root_rrn);
}

void BPTree__update_root(BPTree *tree, int root_rrn) {
  FILE *fp = fopen(ROOT_FILENAME, "r+");

  tree->root = Node__read(root_rrn);

  fprintf(fp, "{Root_rrn: %d}\n", root_rrn);

  fclose(fp);
}
