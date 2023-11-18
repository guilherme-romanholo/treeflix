#include "../../include/bp_tree.h"
#include "../../include/interface.h"
#include "../../include/movie.h"
#include "../../include/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test1() {
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
}

void test2() {
  // Interface__read_movie();
  // Interface__search_key();
  Interface__menu();
}

int main(int argc, char *argv[]) {
  BPTree *tree = BPTree__init();
  List *list = List__init();
  int opc;

  while ((opc = Interface__menu()) != 0) {

    switch (opc) {
    case 1:
      Interface__read_movie(tree, &list);
      break;
    case 2:
      Interface__movie_search(tree, list);
      break;
    default:
      break;
    }
  }

  // Fazer a finalização
  BPTree__free(tree);
  List__destroy(list);

  return EXIT_SUCCESS;
}
