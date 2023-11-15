#include "../../include/bp_tree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  BPTree *tree = BPTree__init();

  tree->insert(tree, "ROM03", 2);
  tree->insert(tree, "AOM03", 1);
  tree->insert(tree, "BOM03", 3);
  tree->insert(tree, "MOM03", 4);
  tree->insert(tree, "COM03", 4);
  tree->insert(tree, "DOM03", 4);
  tree->insert(tree, "EOM03", 4);
  tree->insert(tree, "FOM03", 4);
  tree->insert(tree, "GOM03", 4);
  tree->insert(tree, "HOM03", 4);

  debug();

  return EXIT_SUCCESS;
}
