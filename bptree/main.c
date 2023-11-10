#include "bp_tree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  Node *node_1 = Node__create(1);
  Node *node_2 = Node__create(1);
  Node *node_3 = Node__create(1);

  Node__write(node_1);
  Node__write(node_2);
  Node__write(node_2);

  free(node_1);
  free(node_2);
  free(node_3);

  debug();

  // Node *test_1 = Node__read(0);
  Node *test_2 = Node__read(1);
  //
  // Node__print(test_1);
  Node__print(test_2);
  //
  // free(test_1);
  free(test_2);

  return EXIT_SUCCESS;
}
