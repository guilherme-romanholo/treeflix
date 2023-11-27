#include "../../include/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Nome do arquivo de indice secundário
const char *LIST_FILENAME = "ititle.idx";
// Formatação para escrita no arquivo
const char *LIST_FORMAT_OUT = "{Title: %s, Key: %s}\n";
// Formatação para leitura do arquivo
const char *LIST_FORMAT_IN = "{Title: %[^,], Key: %[^}]}\n";

// Estrutura da lista
struct list {
  char *title;
  char *key;
  struct list *next;
};

// Função responsável por iniciar a lista
List *List__init() {
  List *list = NULL;
  FILE *fp = fopen(LIST_FILENAME, "r");

  // Verifica se o arquivo existe para leitura, se não cria um novo
  if (fp == NULL) {
    fp = fopen(LIST_FILENAME, "w");
  } else {
    list = List__read(fp);
  }

  fclose(fp);

  return list;
}

// Libera a memória da lisa e escreve ela atualizada no arquivo
void List__destroy(List *list) {
  FILE *fp = fopen(LIST_FILENAME, "w");

  List__write(list, fp);

  fclose(fp);
}

// Cria um novo nó para a lista
List *List__create_node(char *key, char *title) {
  List *node = malloc(sizeof(List));

  // Atribui os valores dos campos
  node->title = strdup(title);
  node->key = strdup(key);
  node->next = NULL;

  return node;
}

// Insere um novo elemento na lista de maneira ordenada
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

// Função para buscar um determinado elemento na lista
char *List__search(List *list, char *title) {
  char *key = NULL;

  // Enquanto não chega no final da lista
  while (list) {
    // Compara os valores com o titulo escolhido
    if (!strcasecmp(list->title, title)) {
      key = list->key;
      break;
    }
    list = list->next;
  }

  // Retorna a chave caso encontrada ou NULL caso não
  return key;
}

// Função para ler a lista do arquivo
List *List__read(FILE *fp) {
  List *list = NULL;
  char key[6], title[200];

  // Aloca os nós para cada elemento e insere ele na lista
  while (fscanf(fp, LIST_FORMAT_IN, title, key) != EOF) {
    List *node = List__create_node(key, title);
    List__insert(&list, node);
  }

  return list;
}

// Função para escrever a lista no arquivo
void List__write(List *list, FILE *fp) {
  // Enquanto existirem elementos na lista
  while (list) {
    List *aux = list;

    // Escreve no arquivo
    fprintf(fp, LIST_FORMAT_OUT, list->title, list->key);

    list = list->next;

    // Libera o elemento anterior da memória
    free(aux->title);
    free(aux->key);
    free(aux);
  }
}
