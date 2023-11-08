#include "../include/movie.h"
#include <stdlib.h>
#include <string.h>

struct movie {
  char *pt_title;
  char *title;
  char *director;
  char *year;
  char *country;
  char score;
};

Movie *Movie__create(char *title) {
  Movie *movie = malloc(sizeof(Movie));

  movie->title = strdup(title);

  return movie;
}

void Movie__destroy(Movie *self) {
  if (self) {
    free(self->title);
    free(self);
  }
}
