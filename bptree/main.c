#include "bp_tree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  BPTree *tree = BPTree__create();

  tree->insert(tree->root, "TES01", 4);

  Node *teste = Node__read(0);
  Node__print(teste);

  return EXIT_SUCCESS;
}
