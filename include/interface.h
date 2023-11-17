#ifndef INTERFACE_H
#define INTERFACE_H

#include "movie.h"

void Interface__read_movie();
void Interface__print_movie(Movie *movie);
void Interface__search_key();
int Interface__menu();

#endif // !INTERFACE_H
