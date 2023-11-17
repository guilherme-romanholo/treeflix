#include "../../include/movie.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *MOVIE_FORMAT_OUT = "%s@%s@%s@%s@%s@%s@%d@";
const char *MOVIE_FILENAME = "movies.dat";
const int MOVIE_SIZE = 192;

Movie *Movie__create(char *buffer) {
  Movie *movie = malloc(sizeof(Movie));

  movie->key = strdup(strtok(buffer, "@"));
  movie->pt_title = strdup(strtok(NULL, "@"));
  movie->title = strdup(strtok(NULL, "@"));
  movie->director = strdup(strtok(NULL, "@"));
  movie->year = strdup(strtok(NULL, "@"));
  movie->country = strdup(strtok(NULL, "@"));
  movie->score = atoi(strtok(NULL, "@"));

  return movie;
}

void Movie__destroy(Movie *self) {
  if (self) {
    free(self->key);
    free(self->pt_title);
    free(self->title);
    free(self->director);
    free(self->year);
    free(self->country);
    free(self);
  }
}

void Movie__make_key(Movie *self) {
  if (strlen(self->year) < 4 || strlen(self->director) < 3) {
    printf("Nome do diretor ou ano inválido(s) para criação da chave!\n");
    exit(1);
  }

  char *key = calloc(sizeof(char), 6);

  key[0] = toupper(self->director[0]);
  key[1] = toupper(self->director[1]);
  key[2] = toupper(self->director[2]);
  key[3] = self->year[2];
  key[4] = self->year[3];
  key[5] = '\0';

  self->key = key;
}

char *Movie__bufferize(Movie *movie) {
  char *buffer = calloc(sizeof(char), MOVIE_SIZE);

  int size = sprintf(buffer, MOVIE_FORMAT_OUT, movie->key, movie->pt_title,
                     movie->title, movie->director, movie->year, movie->country,
                     movie->score);

  for (int i = size; i < MOVIE_SIZE; i++)
    buffer[i] = '#';

  return buffer;
}

void Movie__append(Movie *movie) {
  FILE *fp = fopen(MOVIE_FILENAME, "a");
  char *buffer = Movie__bufferize(movie);

  fputs(buffer, fp);

  fclose(fp);
  free(buffer);
}

Movie *Movie__read(int rrn) {
  FILE *fp = fopen(MOVIE_FILENAME, "r");
  Movie *movie = NULL;
  char *buffer = calloc(sizeof(char), MOVIE_SIZE);

  fseek(fp, rrn * MOVIE_SIZE, SEEK_SET);
  fgets(buffer, MOVIE_SIZE, fp);

  movie = Movie__create(buffer);

  free(buffer);
  fclose(fp);

  return movie;
}
