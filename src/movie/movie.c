#include "../../include/movie.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Formato de escrita do filme no arquivo
const char *MOVIE_FORMAT_OUT = "%s@%s@%s@%s@%s@%s@%d@";
// Nome do arquivo de dados
const char *MOVIE_FILENAME = "movies.dat";
// Tamanho máximo de um filme
const int MOVIE_SIZE = 192;

void Movie__init() {
  FILE *fp = fopen(MOVIE_FILENAME, "r");

  // Caso o arquivo não exista cria ele
  if (fp == NULL)
    fp = fopen(MOVIE_FILENAME, "w");

  fclose(fp);
}

// Função responsável por criar um filme
Movie *Movie__create(char *buffer) {
  Movie *movie = malloc(sizeof(Movie));

  // Divide o buffer com strtok e aloca memória para cada campo com o strdup
  movie->key = strdup(strtok(buffer, "@"));
  movie->pt_title = strdup(strtok(NULL, "@"));
  movie->title = strdup(strtok(NULL, "@"));
  movie->director = strdup(strtok(NULL, "@"));
  movie->year = strdup(strtok(NULL, "@"));
  movie->country = strdup(strtok(NULL, "@"));
  movie->score = atoi(strtok(NULL, "@"));

  // Retorna o filme
  return movie;
}

// Libera um filme da memória principal
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

// Função para criar a chave primária com base em um filme
void Movie__make_key(Movie *self) {
  // Caso as informações para criar a chave estejam erradas
  if (strlen(self->year) < 4 || strlen(self->director) < 3) {
    printf("Nome do diretor ou ano inválido(s) para criação da chave!\n");
    exit(1);
  }

  char *key = calloc(sizeof(char), 6);

  // Cria a chave com as especificações dadas
  key[0] = toupper(self->director[0]);
  key[1] = toupper(self->director[1]);
  key[2] = toupper(self->director[2]);
  key[3] = self->year[2];
  key[4] = self->year[3];
  key[5] = '\0';

  // Atribui ao filme a nova chave
  self->key = key;
}

// Função responsável por bufferizar o filme para escrita
char *Movie__bufferize(Movie *movie) {
  char *buffer = calloc(sizeof(char), MOVIE_SIZE);

  // Escreve o filme formatado no buffer
  int size = sprintf(buffer, MOVIE_FORMAT_OUT, movie->key, movie->pt_title,
                     movie->title, movie->director, movie->year, movie->country,
                     movie->score);

  // Preenche o restante dos espaços com o caractere '#'
  for (int i = size; i < MOVIE_SIZE; i++)
    buffer[i] = '#';

  // Retorna o buffer
  return buffer;
}

// Função responsável por adicionar o filme ao arquivo de dados
int Movie__append(Movie *movie) {
  // Bufferiza o filme e encontra seu rrn
  FILE *fp = fopen(MOVIE_FILENAME, "a");
  char *buffer = Movie__bufferize(movie);
  int rrn = ftell(fp) / MOVIE_SIZE;

  // Escreve ele no arquivo
  fputs(buffer, fp);

  // Fecha o arquivo e libera a memória
  free(buffer);
  fclose(fp);

  // Retorna o rrn
  return rrn;
}

// Função responsável por ler um filme
Movie *Movie__read(int rrn) {
  FILE *fp = fopen(MOVIE_FILENAME, "r");
  Movie *movie = NULL;
  char *buffer = calloc(sizeof(char), MOVIE_SIZE);

  // Dá um seek no arquivo até seu rrn
  fseek(fp, rrn * MOVIE_SIZE, SEEK_SET);
  // Lẽ o registro
  fgets(buffer, MOVIE_SIZE, fp);

  // Cria um novo filme com base no buffer
  movie = Movie__create(buffer);

  // Fecha o arquivo e libera a memória
  free(buffer);
  fclose(fp);

  // Retorna o filme
  return movie;
}

// Função responsável por reescrever um filme alterado
void Movie__write(Movie *movie, int rrn) {
  FILE *fp = fopen(MOVIE_FILENAME, "r+");
  char *buffer = Movie__bufferize(movie);

  // Dá um seek na posição do filme
  fseek(fp, rrn * MOVIE_SIZE, SEEK_SET);
  // Reescreve esse filme
  fputs(buffer, fp);

  // Fecha o arquivo e libera a memória
  free(buffer);
  fclose(fp);
}
