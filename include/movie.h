#ifndef MOVIE_H
#define MOVIE_H

typedef struct movie {
  char *key;
  char *pt_title;
  char *title;
  char *director;
  char *year;
  char *country;
  int score;
} Movie;

Movie *Movie__create(char *buffer);
void Movie__destroy(Movie *self);
void Movie__append(Movie *movie);
char *Movie__make_key(Movie *self);

#endif // !MOVIE_H
