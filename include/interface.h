#ifndef INTERFACE_H
#define INTERFACE_H

#include "movie.h"
#include "bp_tree.h"

void Interface__read_movie(BPTree *tree);
void Interface__print_movie(Movie *movie);
int Interface__menu();
void Interface__movie_search(BPTree *tree);
void Interface__movie_search_key(BPTree *tree);

#endif // !INTERFACE_H
