#include "../../include/interface.h"
#include "../../include/movie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Interface__menu() {
  puts("----------------------------------");
  puts("|  _____             __ _ _      |");
  puts("| |_   _| _ ___ ___ / _| (_)_ __ |");
  puts("|   | || '_/ -_) -_)  _| | \\ \\ / |");
  puts("|   |_||_| \\___\\___|_| |_|_/_\\_\\ |");
  puts("|--------------------------------|");
  puts("|                                |");
  puts("|    (1) Cadastrar novo filme    |");
  puts("|    (2) Buscar um filme         |");
  puts("|    (0) Sair                    |");
  puts("|                                |");
  puts("----------------------------------");

  return 1;
}

void Interface__read_movie() {
  Movie *movie = malloc(sizeof(Movie));
  char *buffer = calloc(sizeof(char), 100);
  int size = 0;

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

  Movie__make_key(movie);
  size += 5;

  if (size > 175) {
    puts("Campos ultrapassam o tamanho máximo do registro!");
    exit(1);
  }

  Movie__append(movie);

  free(buffer);
  Movie__destroy(movie);
}

void Interface__search_key() {
  char key[6];
  int rrn;

  scanf("%d", &rrn);

  Movie *movie = Movie__read(rrn);

  Interface__print_movie(movie);

  Movie__destroy(movie);

  // puts("Digite a chave primária do filme:");
  // scanf("%s", key);
}

void Interface__print_movie(Movie *movie) {
  printf("Chave: %s\n", movie->key);
  printf("Título em português: %s\n", movie->pt_title);
  printf("Título original: %s\n", movie->title);
  printf("Diretor: %s\n", movie->director);
  printf("Ano: %s\n", movie->year);
  printf("País: %s\n", movie->country);
  printf("Nota: %d\n", movie->score);
}