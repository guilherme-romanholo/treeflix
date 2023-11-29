#ifndef LIST_H
#define LIST_H
#include <stdio.h>

typedef struct list List;

/* Constructor and Destructor */
List *List__init();
void List__destroy(List *list);

/* Insertion */
List *List__create_node(char *key, char *title);
void List__insert(List **head, List *new_node);

/* Search */
char *List__search(List *list, char *title);

/* File Manipulation */
List *List__read(FILE *fp);
void List__write(List *list, FILE *fp);

#endif // !LIST_H
