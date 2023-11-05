#ifndef MOVIE_H
#define MOVIE_H

typedef struct movie Movie;
Movie *Movie__create(char *pt_title);
void Movie__destroy(Movie *self);
char *Movie__getTitle(Movie *self);

#endif // !MOVIE_H
