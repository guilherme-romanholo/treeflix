#ifndef INTERFACE_H
#define INTERFACE_H

#include "movie.h"
#include "bp_tree.h"

void Interface__read_movie(BPTree *tree);
void Interface__print_movie(Movie *movie);
void Interface__search_key();
int Interface__menu();

#endif // !INTERFACE_H
