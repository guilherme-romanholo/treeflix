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

/* Constructor and Destructor */
void Movie__init();
Movie *Movie__create(char *buffer);
void Movie__destroy(Movie *self);

/* File Manipulation */
int Movie__append(Movie *movie);
Movie *Movie__read(int rrn);
void Movie__write(Movie *movie, int rrn);

/* Make Key */
void Movie__make_key(Movie *self);

#endif // !MOVIE_H
