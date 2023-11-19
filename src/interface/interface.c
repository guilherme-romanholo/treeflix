#include "../../include/interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Interface__menu() {
  int opc;

  do {
    puts("----------------------------------");
    puts("|  _____             __ _ _      |");
    puts("| |_   _| _ ___ ___ / _| (_)_ __ |");
    puts("|   | || '_/ -_) -_)  _| | \\ \\ / |");
    puts("|   |_||_| \\___\\___|_| |_|_/_\\_\\ |");
    puts("|--------------------------------|");
    puts("|                                |");
    puts("|    (1) Cadastrar novo filme    |");
    puts("|    (2) Buscar um filme         |");
    puts("|    (3) Atualizar nota          |");
    puts("|    (4) Listar filmes           |");
    puts("|    (0) Sair                    |");
    puts("|                                |");
    puts("----------------------------------");
    printf(">> ");
    scanf("%d", &opc);
  } while (opc < 0 || opc > 6);

  return opc;
}

void Interface__read_movie(BPTree *tree, List **list) {
  Movie *movie = malloc(sizeof(Movie));
  char *buffer = calloc(sizeof(char), 100);
  int size = 0;

  // Coleta de informações do usuário
  puts("Digite o nome do filme em português: ");
  scanf(" %[^\n]", buffer);
  movie->pt_title = strdup(buffer);
  size += strlen(buffer);

  puts("Digite o nome do filme original: ");
  scanf(" %[^\n]", buffer);
  movie->title = strdup(buffer);
  size += strlen(buffer);

  puts("Digite o nome do diretor (Sobrenome, Nome): ");
  scanf(" %[^\n]", buffer);
  movie->director = strdup(buffer);
  size += strlen(buffer);

  puts("Digite o ano de lançamento: ");
  scanf(" %[^\n]", buffer);
  movie->year = strdup(buffer);
  size += strlen(buffer);

  puts("Digite o país de origem: ");
  scanf(" %[^\n]", buffer);
  movie->country = strdup(buffer);
  size += strlen(buffer);

  puts("Digite a nota do filme (0 - 9): ");
  scanf("%d", &movie->score);
  size += 1;

  // Criação da chave
  Movie__make_key(movie);
  size += 5;

  if (size > 175) {
    puts("Campos ultrapassam o tamanho máximo do registro!");
    // Em vez de exit(1) fazer uma função de finalizaçao
    exit(1);
  }

  if (Node__search_key(movie->key) != -1) {
    free(buffer);
    Movie__destroy(movie);
    puts("O filme já existe na base de dados!");
    return;
  }

  // Inserção no arquivo de dados
  int movie_rrn = Movie__append(movie);
  tree->insert(tree, movie->key, movie_rrn);
  List__insert(list, List__create_node(movie->key, movie->pt_title));

  free(buffer);
  Movie__destroy(movie);
}

void Interface__movie_search(BPTree *tree, List *list) {
  int opc;

  do {
    puts("----------------------------------");
    puts("|  _____             __ _ _      |");
    puts("| |_   _| _ ___ ___ / _| (_)_ __ |");
    puts("|   | || '_/ -_) -_)  _| | \\ \\ / |");
    puts("|   |_||_| \\___\\___|_| |_|_/_\\_\\ |");
    puts("|--------------------------------|");
    puts("|                                |");
    puts("|    (1) Buscar pela chave       |");
    puts("|    (2) Buscar pelo título      |");
    puts("|    (0) Voltar                  |");
    puts("|                                |");
    puts("----------------------------------");
    printf(">> ");
    scanf("%d", &opc);
  } while (opc < 0 || opc > 2);

  switch (opc) {
  case 1:
    Interface__movie_search_key(tree);
    return;
  case 2:
    Interface__movie_search_title(list);
    return;
  default:
    return;
  }
}

void Interface__movie_search_key(BPTree *tree) {
  char key[6];
  int rrn;
  Movie *movie;

  puts("Digite a chave do filme procurado:");
  scanf(" %s", key);

  rrn = Node__search_key(key);

  if (rrn == -1) {
    puts("O filme buscado não existe!");
    return;
  }

  movie = Movie__read(rrn);
  Interface__print_movie(movie);

  Movie__destroy(movie);
}

void Interface__movie_search_title(List *list) {
  char title[200], *key;
  int rrn;
  Movie *movie;

  puts("Digite o título do filme procurado:");
  scanf(" %s", title);

  key = List__search(list, title);

  if (key == NULL) {
    puts("O filme buscado não existe!");
    return;
  }

  rrn = Node__search_key(key);

  movie = Movie__read(rrn);
  Interface__print_movie(movie);

  Movie__destroy(movie);

  // Problema de buffer
  fgetc(stdin);
}

void Interface__update_score() {
  char key[6];
  int rrn, score;
  Movie *movie;

  puts("Digite a chave do filme procurado:");
  scanf(" %s", key);

  rrn = Node__search_key(key);

  if (rrn == -1) {
    puts("O filme buscado não existe!");
    return;
  }

  movie = Movie__read(rrn);

  printf("Digite a nova nota para o filme %s:\n", movie->title);
  scanf("%d", &score);

  movie->score = score;

  Movie__write(movie, rrn);
  Movie__destroy(movie);

  puts("Nota atualizada com sucesso!");
}

void Interface__movie_list(BPTree *tree) {
  int opc;

  do {
    puts("----------------------------------");
    puts("|  _____             __ _ _      |");
    puts("| |_   _| _ ___ ___ / _| (_)_ __ |");
    puts("|   | || '_/ -_) -_)  _| | \\ \\ / |");
    puts("|   |_||_| \\___\\___|_| |_|_/_\\_\\ |");
    puts("|--------------------------------|");
    puts("|                                |");
    puts("|    (1) Listar um range         |");
    puts("|    (2) Listar todos os filmes  |");
    puts("|    (0) Voltar                  |");
    puts("|                                |");
    puts("----------------------------------");
    printf(">> ");
    scanf("%d", &opc);
  } while (opc < 0 || opc > 2);

  switch (opc) {
  case 1:
    Interface__movie_list_range(tree);
    return;
  case 2:
    Interface__movie_list_all(tree);
    return;
  default:
    return;
  }
}

void Interface__movie_list_range(BPTree *tree) {
  Node *node;
  char key[6];

  puts("Digite a chave para o range da listagem:");
  scanf(" %s", key);

  node = Node__search(key);

  Node__list_range(node, key);
}

void Interface__movie_list_all(BPTree *tree) {
  char *key = Node__get_lower_key(tree);

  Node__list_range(tree->root, key);
}

void Interface__print_movie(Movie *movie) {
  // Colocar os fgetc e clear aqui
  printf("Chave: %s\n", movie->key);
  printf("Título em português: %s\n", movie->pt_title);
  printf("Título original: %s\n", movie->title);
  printf("Diretor: %s\n", movie->director);
  printf("Ano: %s\n", movie->year);
  printf("País: %s\n", movie->country);
  printf("Nota: %d\n", movie->score);
}
