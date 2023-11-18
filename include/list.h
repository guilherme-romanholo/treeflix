#ifndef LIST_H
#define LIST_H
#include <stdio.h>

typedef struct list List;

List *List__init();
void List__destroy(List *list);
List *List__create_node(char *key, char *title);
void List__insert(List **head, List *new_node);
char *List__search(List *list, char *title);
List *List__read(FILE *fp);
void List__write(List *list, FILE *fp);

#endif // !LIST_H
