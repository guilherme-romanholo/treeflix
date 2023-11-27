#include "../../include/interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função responsável por printar o menu principal
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
    puts("|    (5) Printar arvore          |");
    puts("|    (0) Sair                    |");
    puts("|                                |");
    puts("----------------------------------");
    printf(">> ");
    scanf("%d", &opc);
  } while (opc < 0 || opc > 6);

  return opc;
}

// Função responsável para ler um filme de entrada
void Interface__read_movie(BPTree *tree, List **list) {
  Movie *movie = malloc(sizeof(Movie));
  char *buffer = calloc(sizeof(char), 100);
  int size = 0;

  puts("----------------------------------");
  puts("|  _____             __ _ _      |");
  puts("| |_   _| _ ___ ___ / _| (_)_ __ |");
  puts("|   | || '_/ -_) -_)  _| | \\ \\ / |");
  puts("|   |_||_| \\___\\___|_| |_|_/_\\_\\ |");
  puts("|--------------------------------|\n");

  // Coleta de informações do usuário e conta o tamanho total do buffer

  printf("Digite o nome do filme em português:\n>> ");
  scanf(" %[^\n]", buffer);
  movie->pt_title = strdup(buffer);
  size += strlen(buffer);

  printf("Digite o nome do filme original:\n>> ");
  scanf(" %[^\n]", buffer);
  movie->title = strdup(buffer);
  size += strlen(buffer);

  printf("Digite o nome do diretor (Sobrenome, Nome):\n>> ");
  scanf(" %[^\n]", buffer);
  movie->director = strdup(buffer);
  size += strlen(buffer);

  printf("Digite o ano de lançamento:\n>> ");
  scanf(" %[^\n]", buffer);
  movie->year = strdup(buffer);
  size += strlen(buffer);

  printf("Digite o país de origem:\n>> ");
  scanf(" %[^\n]", buffer);
  movie->country = strdup(buffer);
  size += strlen(buffer);

  printf("Digite a nota do filme (0 - 9):\n>> ");
  scanf("%d", &movie->score);
  size += 1;

  // Criação da chave e aumento do tamanho do buffer
  Movie__make_key(movie);
  size += 5;

  // Caso a quantidade de letras das informações ultrapasse
  // o total permitido ele retorna um erro
  if (size > 175) {
    free(buffer);
    Movie__destroy(movie);
    puts("Campos ultrapassam o tamanho máximo do registro!");
    return;
  }

  // Caso o filme já exista na base de dados, ele retorna um erro também
  if (Node__search_key(movie->key) != -1) {
    free(buffer);
    Movie__destroy(movie);
    puts("O filme já existe na base de dados!");
    return;
  }

  // Inserção no arquivo de dados do novo filme
  int movie_rrn = Movie__append(movie);
  // Inserção na arvore
  tree->insert(tree, movie->key, movie_rrn);
  // Inserção na lista secundária
  List__insert(list, List__create_node(movie->key, movie->pt_title));

  // Libera a memória
  free(buffer);
  Movie__destroy(movie);

  puts("Filme cadastrado com sucesso!");
}

// Função responsável por procurar um filme
void Interface__movie_search(BPTree *tree, List *list) {
  int opc;

  // Mostra as opções
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

  // Escolhe o método de busca
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

// Função responsável por buscar um filme pela chave
void Interface__movie_search_key(BPTree *tree) {
  char key[6];
  int rrn;
  Movie *movie;

  // Input da chave
  puts("Digite a chave do filme procurado:");
  scanf(" %s", key);

  // Procura o rrn atrelado a chave fornecida
  rrn = Node__search_key(key);

  // Caso o elemento não exista
  if (rrn == -1) {
    puts("O filme buscado não existe!");
    return;
  }

  // Lê o filme e printa as informações referentes a ele
  movie = Movie__read(rrn);
  Interface__print_movie(movie);

  // Libera a memória
  Movie__destroy(movie);
}

// Função responsável por buscar um filme pelo título
void Interface__movie_search_title(List *list) {
  char title[200], *key;
  int rrn;
  Movie *movie;

  // Input do título
  puts("Digite o título em português do filme procurado:");
  scanf(" %[^\n]", title);

  // Procura o título do filme na lista, recebendo a chave
  key = List__search(list, title);

  // Caso o elemento não exista
  if (key == NULL) {
    puts("O filme buscado não existe!");
    return;
  }

  // Procura o rrn atrelado a chave fornecida
  rrn = Node__search_key(key);

  // Lê o filme e printa as informações referentes a ele
  movie = Movie__read(rrn);
  Interface__print_movie(movie);

  // Libera a memória
  Movie__destroy(movie);

  // Problema de buffer
  fgetc(stdin);
}

// Função responsável por atualizar a nota
void Interface__update_score() {
  char key[6];
  int rrn, score;
  Movie *movie;

  // Input da chave
  puts("Digite a chave do filme procurado:");
  scanf(" %s", key);

  // Procura o rrn atrelado a chave
  rrn = Node__search_key(key);

  // Caso a chave não seja encontrada
  if (rrn == -1) {
    puts("O filme buscado não existe!");
    return;
  }

  // Lê o filme com seu rrn
  movie = Movie__read(rrn);

  // Input da nova nota
  printf("Digite a nova nota para o filme %s:\n", movie->title);
  scanf("%d", &score);

  // Atualiza a nota
  movie->score = score;

  // Escreve o filme atualizado e libera a memória
  Movie__write(movie, rrn);
  Movie__destroy(movie);

  puts("Nota atualizada com sucesso!");
}

// Função responsável por listar os filmes
void Interface__movie_list(BPTree *tree) {
  int opc;

  // Mostra as opções
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

  // Escolhe o método de listagem
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

// Função para listar os filmes de um determinado range
void Interface__movie_list_range(BPTree *tree) {
  Node *node;
  char key[6];

  // Escolhe a chave de inicio
  puts("Digite a chave para o range da listagem:");
  scanf(" %s", key);

  // Procura o nó folha que ela esta localizada
  node = Node__search(key);

  // Lista todas as chaves a partir dela
  Node__list_range(node, key);
}

// Função para listar todos os filmes
void Interface__movie_list_all(BPTree *tree) {
  // Acha a menor chave da árvore
  char *key = Node__get_lower_key(tree);

  // Lista todas as outras a partir dela
  Node__list_range(tree->root, key);
}

// Função responsável por printar o filme formatado
void Interface__print_movie(Movie *movie) {
  puts("----------------------------------");
  printf("Chave: %s\n", movie->key);
  printf("Título em português: %s\n", movie->pt_title);
  printf("Título original: %s\n", movie->title);
  printf("Diretor: %s\n", movie->director);
  printf("Ano: %s\n", movie->year);
  printf("País: %s\n", movie->country);
  printf("Nota: %d\n", movie->score);
  puts("----------------------------------");
}
