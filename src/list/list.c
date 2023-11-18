#include "../../include/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

const char *LIST_FILENAME = "ititle.idx";
const char *LIST_FORMAT_OUT = "{Title: %s, Key: %s}\n";
const char *LIST_FORMAT_IN = "{Title: %[^,], Key: %[^}]}\n";

struct list {
  char *title;
  char *key;
  struct list *next;
};

List *List__init() {
  List *list = NULL;
  FILE *fp = fopen(LIST_FILENAME, "r");

  if (fp == NULL) {
    fp = fopen(LIST_FILENAME, "w");
  } else {
    list = List__read(fp);
  }

  fclose(fp);

  return list;
}

void List__destroy(List *list) {
  FILE *fp = fopen(LIST_FILENAME, "w");

  List__write(list, fp);

  fclose(fp);
}

List *List__create_node(char *key, char *title) {
  List *node = malloc(sizeof(List));

  node->title = strdup(title);
  node->key = strdup(key);
  node->next = NULL;

  return node;
}

void List__insert(List **head, List *new_node) {
  // Caso a lista esteja vazia ou o primeiro elemento seja substituído
  if (*head == NULL || strcmp((*head)->title, new_node->title) >= 0) {
    // Insere o elemento na cabeça da lista
    new_node->next = *head;
    *head = new_node;
    return;
  }

  List *current = *head;

  // Procura o local de inserção do elemento
  while (current->next != NULL && strcmp(current->title, new_node->title) < 0)
    current = current->next;

  // Insere o elemento no local encontrado
  new_node->next = current->next;
  current->next = new_node;
}

char *List__search(List *list, char *title) {
  char *key = NULL;

  while (list) {
    if (!strcasecmp(list->title, title)) {
      key = list->key;
      break;
    }
    list = list->next;
  }

  return key;
}

List *List__read(FILE *fp) {
  List *list = NULL;
  char key[6], title[200];

  while (fscanf(fp, LIST_FORMAT_IN, title, key) != EOF) {
    List *node = List__create_node(key, title);
    List__insert(&list, node);
  }

  return list;
}

void List__write(List *list, FILE *fp) {
  while (list) {
    List *aux = list;

    fprintf(fp, LIST_FORMAT_OUT, list->title, list->key);

    list = list->next;
    
    free(aux->title);
    free(aux->key);
    free(aux);
  }
}
