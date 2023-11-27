#include "../../include/bp_tree.h"
#include "../../include/interface.h"
#include "../../include/movie.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ORDER 4

// Nome do arquivo
const char *TREE_FILENAME = "ibtree.idx";

// Estrutura de um nó
struct node {
  int rrn;
  int is_leaf;
  char keys[ORDER][6];
  int data_rrn[ORDER];
  int children[ORDER + 1];
  int num_keys;
  int parent;
  int next_node;
};

/* CONSTRUCTOR */

// Cria um novo nó na memória principal
Node *Node__create(int is_leaf) {
  Node *node = malloc(sizeof(Node));

  node->rrn = -1;
  node->is_leaf = is_leaf;
  node->num_keys = 0;
  node->parent = -1;
  node->next_node = -1;

  for (int i = 0; i < ORDER; i++) {
    strcpy(node->keys[i], "-----");
    node->data_rrn[i] = -1;
    node->children[i] = -1;
  }
  node->children[ORDER] = -1;

  // Adiciona o novo nó no arquivo
  Node__append(node);

  return node;
}

/* FILE MANIPULATION */

// Lê um nó de um arquivo dado seu rrn
Node *Node__read(int rrn) {
  FILE *fp = fopen(TREE_FILENAME, "rb");
  Node *node = malloc(sizeof(Node));

  // Procura a posição, como o arquivo é binário
  // procuramos com base no tamanho da struct
  fseek(fp, rrn * sizeof(Node), SEEK_SET);

  fread(node, sizeof(Node), 1, fp);

  fclose(fp);

  return node;
}

// Adiciona o novo nó no final do arquivo
void Node__append(Node *node) {
  FILE *fp = fopen(TREE_FILENAME, "ab");

  // Atribui um rrn ao novo nó
  node->rrn = ftell(fp) / sizeof(Node);

  fwrite(node, sizeof(Node), 1, fp);

  fclose(fp);
}

// Reescreve um nó modificado no arquivo
void Node__write(Node *node, int rrn) {
  FILE *fp = fopen(TREE_FILENAME, "rb+");

  // Procura sua posição e reescreve
  fseek(fp, rrn * sizeof(Node), SEEK_SET);

  fwrite(node, sizeof(Node), 1, fp);

  fclose(fp);
}

/* UTILITIES */

// Função para printar os nós da árvore
void debug() {
  FILE *fp = fopen(TREE_FILENAME, "rb");
  Node *node = malloc(sizeof(Node));

  while (fread(node, sizeof(Node), 1, fp) == 1)
    Node__print(node);

  fclose(fp);
  free(node);
}

// Função para listar um determinado range de chaves
void Node__list_range(Node *node, char *key) {
  Movie *movie;
  int pos = 0, node_rrn;

  // Recebemos como parâmetro o nó em que a primeira
  // chave do range está localizada, então iteramos
  // até a sua posição
  while (strcmp(node->keys[pos], key) > 0)
    pos++;

  // Printamos todos os filmes a partir dessa chave
  for (int i = pos + 1; i < node->num_keys; i++) {
    movie = Movie__read(node->data_rrn[i]);
    Interface__print_movie(movie);
    Movie__destroy(movie);
  }

  // Pegamos o rrn da próxima folha
  node_rrn = node->next_node;
  node = Node__read(node_rrn);

  // Fazemos esse processo novamente até não
  // existirem mais folhas
  while (node_rrn != -1) {

    for (int i = 0; i < node->num_keys; i++) {
      movie = Movie__read(node->data_rrn[i]);
      Interface__print_movie(movie);
      Movie__destroy(movie);
    }

    node_rrn = node->next_node;
    free(node);
    node = Node__read(node_rrn);
  }
}

// Função responsável por retoranar a menor
// chave da Árvore B+
char *Node__get_lower_key(BPTree *tree) {
  Node *root = tree->root;
  int child_rrn;
  char *key;

  // Percorre a árvore até chegar na
  // folha localiza mais a esquerda
  while (root->is_leaf == 0) {
    child_rrn = root->children[0];
    free(root);
    root = Node__read(child_rrn);
  }

  // Retorna essa chave
  key = strdup(root->keys[0]);

  return key;
}

// Função responsável por printar um nó da árvore
void Node__print(Node *node) {
  printf("--------------------\n");
  printf("RRN: %d\n", node->rrn);
  printf("Parent: %d\n", node->parent);
  printf("Num keys: %d\n", node->num_keys);
  printf("Keys: ");
  for (int i = 0; i < ORDER; i++)
    printf("%s ", node->keys[i]);

  printf("\nData RRN: ");
  for (int i = 0; i < ORDER; i++)
    printf("%d ", node->data_rrn[i]);

  printf("\nChildren RRN: ");
  for (int i = 0; i < ORDER + 1; i++)
    printf("%d ", node->children[i]);

  printf("\nLeaf: %d\n", node->is_leaf);
  printf("Next node: %d\n", node->next_node);
}

// Função responsável por atualizar o campo
// parent dos filhos de um determinado nó
void Node__update_parent(Node *node) {
  int child_size = node->num_keys + 1;

  for (int i = 0; i < child_size; i++) {
    // Carrega um filoh do pai atual
    Node *child = Node__read(node->children[i]);
    // Faz seu filho apontar para ele
    child->parent = node->rrn;
    // Reescreve o filho
    Node__write(child, child->rrn);
    free(child);
  }
}

/* SPLIT */

// Função para fazer split em um nó folha
void Node__split_leaf(Node *dest, Node *ori) {
  int mid, child_size, num_keys;

  // Encontra a chave do meio
  mid = ((int)ceil((float)ori->num_keys / 2)) - 1;
  // Atribui o numero de filhos e de chaves
  child_size = ori->num_keys + 1;
  num_keys = ori->num_keys;

  // Faz a iteração de mid + 1 para mover as chaves de nó
  for (int i = mid + 1, j = 0; i < num_keys; i++, j++) {
    strcpy(dest->keys[j], ori->keys[i]);
    strcpy(ori->keys[i], "-----");

    dest->data_rrn[j] = ori->data_rrn[i];
    ori->data_rrn[i] = -1;

    dest->num_keys++;
    ori->num_keys--;
  }
}

// Função para fazer split em um nó interno
char *Node__split_internal(Node *dest, Node *ori) {
  int mid, child_size, num_keys;
  char *promoted_key = calloc(sizeof(char), 6);

  // Encontra a chave do meio
  mid = ((int)ceil((float)ori->num_keys / 2)) - 1;
  // Atribui o numero de filhos e de chaves
  child_size = ori->num_keys + 1;
  num_keys = ori->num_keys;

  // Faz a iteração de mid + 1 para mover as chaves de nó
  for (int i = mid + 1, j = 0; i < num_keys; i++, j++) {
    strcpy(dest->keys[j], ori->keys[i]);
    strcpy(ori->keys[i], "-----");

    dest->num_keys++;
    ori->num_keys--;
  }

  // Escolhe a primeira chave do novo nó para ser promovida
  strcpy(promoted_key, dest->keys[0]);

  // Elimina essa chave promovida do novo nó interno
  for (int i = 0; i < dest->num_keys; i++)
    strcpy(dest->keys[i], dest->keys[i + 1]);

  dest->num_keys--;

  // Faz a iteração de mid + 1 para mover os filhos
  for (int i = mid + 2, j = 0; i < child_size; i++, j++) {
    dest->children[j] = ori->children[i];
    ori->children[i] = -1;
  }

  // Retorna a chave promovida
  return promoted_key;
}

/* SEARCH */

// Busca um nó folha com base em uma chave
Node *Node__search(char *key) {
  // Carrega a raiz da árvore
  Node *cur = BPTree__read_root();

  // Faz as iterações até achar uma folha
  while (cur->is_leaf == 0) {
    Node *tmp = cur;
    int num_keys = cur->num_keys;

    for (int i = 0; i < num_keys; i++) {

      if (!strcmp(key, cur->keys[i])) {
        // Caso a chave seja igual, carrega o filho da direita
        cur = Node__read(cur->children[i + 1]);
        break;
      } else if (strcmp(key, cur->keys[i]) < 0) {
        // Caso a chave seja menor, carrega o filho da esquerda
        cur = Node__read(cur->children[i]);
        break;
      } else if (i + 1 == cur->num_keys + 1) {
        // Caso seja a ultima chave, carrega o filho da direita
        cur = Node__read(cur->children[i + 1]);
        break;
      }
    }

    // Libera o nó antigo
    free(tmp);
  }

  // Retorna a folha encontrada
  return cur;
}

// Função responsável por procurar uma chave
int Node__search_key(char *key) {
  // Encontra a folha em que a chave supostamente
  // esta localizada
  Node *node = Node__search(key);
  int data_rrn = -1;

  // Procura até encontrar a chave e armazena seu rrn
  for (int i = 0; i < node->num_keys; i++) {
    if (!strcmp(key, node->keys[i])) {
      data_rrn = node->data_rrn[i];
      break;
    }
  }

  free(node);
  // Retorna o rrn ou -1 caso a chave não exista
  return data_rrn;
}

/* INSERTION */

// Função responsável pela inserção de uma nova chave
void Node__insert(BPTree *tree, char *key, int data_rrn) {
  // Procura o nó folha para inserção
  Node *old_node = Node__search(key);
  // Insere nesse nó folha
  Node__insert_at_leaf(old_node, key, data_rrn);

  // Caso o número de chaves seja igual a ordem é
  // preciso realizar um split
  if (old_node->num_keys == ORDER) {
    // Cria um novo nó folha
    Node *new_node = Node__create(1);

    // Atualiza os seus apontamentos
    new_node->parent = old_node->parent;
    new_node->next_node = old_node->next_node;
    old_node->next_node = new_node->rrn;

    // Realiza a distruibuição das chaves
    Node__split_leaf(new_node, old_node);

    // Atualiza os novos nós no arquivo
    Node__write(new_node, new_node->rrn);
    Node__write(old_node, old_node->rrn);

    // Procura recursivamente se é preciso inserir no pai
    Node__insert_in_parent(tree, old_node, new_node, new_node->keys[0]);
  }

  // Atualiza a nova raiz preventivamente
  free(tree->root);
  tree->root = BPTree__read_root();
}

// Função responsável por inserir a nova chave em uma folha
void Node__insert_at_leaf(Node *leaf, char *key, int data_rrn) {
  // Verifica se a folha está vazia
  if (leaf->num_keys != 0) {

    // Procura posição para inserir a chave
    for (int i = 0; i < leaf->num_keys; i++) {

      // Caso a chave ja exista não faz nada
      if (!strcmp(key, leaf->keys[i])) {
        break;
      } else if (strcmp(key, leaf->keys[i]) < 0) {
        // Caso a chave seja menor que a atual faz um shift
        // das chaves na frente
        for (int j = leaf->num_keys; j > i; j--) {
          strcpy(leaf->keys[j], leaf->keys[j - 1]);
          leaf->data_rrn[j] = leaf->data_rrn[j - 1];
        }

        // Insere a nova chave nessa posição
        strcpy(leaf->keys[i], key);
        leaf->num_keys++;
        leaf->data_rrn[i] = data_rrn;

        break;
      } else if (i + 1 == leaf->num_keys) {
        // Caso a ultima alternativa for inserir a chave no final
        strcpy(leaf->keys[i + 1], key);
        leaf->data_rrn[i + 1] = data_rrn;
        leaf->num_keys++;
        break;
      }
    }
  } else {
    // Insere na primeira posição caso esteja vazia a folha
    strcpy(leaf->keys[0], key);
    leaf->data_rrn[0] = data_rrn;
    leaf->num_keys++;
  }

  // Atualiza o nó folha no arquivo
  Node__write(leaf, leaf->rrn);
}

// Função responsável por inserir nos nós internos
void Node__insert_in_parent(BPTree *tree, Node *old_node, Node *new_node,
                            char *promoted_key) {

  // Caso o nó antigo seja a raiz
  if (tree->root->rrn == old_node->rrn) {
    // Cria uma nova raiz
    Node *new_root = Node__create(0);

    // Copia a chave promovida nela
    strcpy(new_root->keys[0], promoted_key);

    // Atualiza os filhos
    new_root->children[0] = old_node->rrn;
    new_root->children[1] = new_node->rrn;

    // Atualiza os pais
    old_node->parent = new_root->rrn;
    new_node->parent = new_root->rrn;

    // Aumenta o numero de chaves
    new_root->num_keys++;

    // Reescreve no arquivo os nós atualizados
    Node__write(old_node, old_node->rrn);
    Node__write(new_node, new_node->rrn);
    Node__write(new_root, new_root->rrn);

    // Atualiza a raiz
    tree->update_root(tree, new_root->rrn);

    return;
  }

  // Lê o pai do nó que vai sofrer split
  Node *parent_node = Node__read(old_node->parent);
  int child_size = parent_node->num_keys + 1;

  // Itera por todos os filhos do nó pai
  for (int i = 0; i < child_size; i++) {

    // Verificamos em qual posição o nó antigo está localizado
    if (parent_node->children[i] == old_node->rrn) {
      int pos = 0;

      // Achamos a posição de inserção da chave promovida
      for (int j = 0; j < parent_node->num_keys; j++)
        if (strcmp(promoted_key, parent_node->keys[j]) > 0)
          pos++;

      // Fazemos um shift nas chaves
      for (int j = parent_node->num_keys; j > pos; j--)
        strcpy(parent_node->keys[j], parent_node->keys[j - 1]);

      // Fazemos um shift nos filhos
      for (int j = child_size; j > (pos + 1); j--)
        parent_node->children[j] = parent_node->children[j - 1];

      // Copiamos a chave promovida e atribuimos seu filho
      strcpy(parent_node->keys[pos], promoted_key);
      parent_node->children[(pos + 1)] = new_node->rrn;

      // Aumentamos o número de chaves
      parent_node->num_keys++;

      // Escreve as mudanças no arquivo
      Node__write(parent_node, parent_node->rrn);
    }

    // Caso seja necessário fazer um split no nó interno
    if (parent_node->num_keys == ORDER) {
      // Cria um nó auxiliar
      Node *parent_dash = Node__create(0);

      // Atualiza o apontamento do pai
      parent_dash->parent = parent_node->parent;

      // Redistribui as chaves
      promoted_key = Node__split_internal(parent_dash, parent_node);

      // Atualiza o apontamento dos novos filhos
      Node__update_parent(parent_node);
      Node__update_parent(parent_dash);

      // Reescreve as mudanças
      Node__write(parent_node, parent_node->rrn);
      Node__write(parent_dash, parent_dash->rrn);

      // Chama recursivamente para subir um nível na árvore
      Node__insert_in_parent(tree, parent_node, parent_dash, promoted_key);
    }
  }
}
