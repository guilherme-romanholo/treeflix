#include "../../include/interface.h"
#include "../../include/movie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  movie->key = strdup(Movie__make_key(movie));
  size += 5;

  if (size > 175) {
    puts("Campos ultrapassam o tamanho máximo do registro!");
    exit(1);
  }

  printf("Problema no append\n");
  fgetc(stdin);

  Movie__append(movie);

  free(buffer);
  Movie__destroy(movie);
}
