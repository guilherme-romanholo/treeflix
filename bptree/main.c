#include "bp_tree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  BPTree *tree = BPTree__init();

  tree->insert(tree->root, "TES01", 4);
  tree->insert(tree->root, "AAA01", 2);
  tree->insert(tree->root, "BAA01", 1);
  tree->insert(tree->root, "CAA01", 3);

  Node *teste = Node__read(0);
  Node__print(teste);

  return EXIT_SUCCESS;
}
