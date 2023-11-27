#include "../../include/bp_tree.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *ROOT_FILENAME = "root.dat";

/* CONSTRUCTOR */

// Função responsável por iniciar uma Árvore B+
BPTree *BPTree__init() {
  BPTree *tree = malloc(sizeof(BPTree));

  // Inicializa os campos da struct
  tree->root = BPTree__read_root();
  tree->insert = Node__insert;
  tree->search = Node__search;
  tree->update_root = BPTree__update_root;

  // Verifica se existe uma raiz
  if (tree->root == NULL) {
    // Caso não exista, ele cria uma raiz
    FILE *fp = fopen(ROOT_FILENAME, "w");
    fprintf(fp, "{Root_rrn: %d}\n", 0);
    fclose(fp);

    tree->root = Node__create(1);
  }

  return tree;
}

/* DESTRUCTOR */

// Função responsável por liberar a memória usada
void BPTree__free(BPTree *tree) {
  free(tree->root);
  free(tree);
}

/* ROOT MANIPULATION */

// Função responsável por ler o arquivo que contem a raiz
Node *BPTree__read_root() {
  FILE *fp = fopen(ROOT_FILENAME, "r");
  int root_rrn;

  // Caso o arquivo não exista, nossa árvore não possui raiz
  if (fp == NULL)
    return NULL;

  // Lê o rrn da raiz do arquivo
  fscanf(fp, "{Root_rrn: %d}\n", &root_rrn);

  fclose(fp);

  // Retorna o nó da raiz
  return Node__read(root_rrn);
}

// Função responsável por atualizar a raiz no arquivo e em memória
void BPTree__update_root(BPTree *tree, int root_rrn) {
  FILE *fp = fopen(ROOT_FILENAME, "r+");

  // Atualiza a nova raiz em memoria
  tree->root = Node__read(root_rrn);

  // Atualiza a nova raiz em arquivo
  fprintf(fp, "{Root_rrn: %d}\n", root_rrn);

  fclose(fp);
}
