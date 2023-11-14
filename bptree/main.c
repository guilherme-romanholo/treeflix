#include "bp_tree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  BPTree *tree = BPTree__init();

  tree->insert("ROM03", 2);
  tree->insert("AOM03", 1);
  tree->insert("BOM03", 3);
  tree->insert("MOM03", 4);
  tree->insert("COM03", 4);
  tree->insert("DOM03", 4);
  tree->insert("EOM03", 4);
  tree->insert("FOM03", 4);
  tree->insert("GOM03", 4);
  tree->insert("HOM03", 4);

  debug();

  return EXIT_SUCCESS;
}
