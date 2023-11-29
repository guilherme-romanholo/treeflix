#include "../../include/bp_tree.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  BPTree *tree = BPTree__init();
  FILE *fp = fopen("input.txt", "r");

  int lines = 0;
  while (!feof(fp)) {
    int ch = fgetc(fp);
    if (ch == '\n') {
      lines++;
    }
  }
  fseek(fp, 0, SEEK_SET);
  printf("lines = %d\n", lines);

  char string[6];
  int num;
  for (int i = 1; i <= lines; i++) {
    fscanf(fp, "%5s,%d\n", string, &num);
    tree->insert(tree, string, num);

    long prev = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    for (int j = 1; j <= i; j++) {
      fscanf(fp, "%5s,%d\n", string, &num);
      int ret = Node__search_key(string);
      printf("%s, step %d, entry %d - %s\n", string, i, j,
             ret == num ? "OK" : "ERROR");
      if (ret != num) {
        printf("ret = %d\n", ret);
        exit(EXIT_FAILURE);
      }
    }
    fseek(fp, prev, SEEK_SET);
    putchar('\n');
  }

  BPTree__free(tree);
  return EXIT_SUCCESS;
}
