#include "../../include/bp_tree.h"
#include "../../include/interface.h"
#include "../../include/list.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  BPTree *tree = BPTree__init();
  List *list = List__init();
  int opc;

  while ((opc = Interface__menu()) != 0) {
    system("clear");

    switch (opc) {
    case 1:
      Interface__read_movie(tree, &list);
      break;
    case 2:
      Interface__movie_search(tree, list);
      break;
    case 3:
      Interface__update_score();
      break;
    case 4:
      Interface__movie_list(tree);
      break;
    default:
      break;
    }

    fgetc(stdin);
  }

  // Fazer a finalização
  BPTree__free(tree);
  List__destroy(list);

  return 0;
}
