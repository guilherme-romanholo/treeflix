#ifndef INTERFACE_H
#define INTERFACE_H

#include "bp_tree.h"
#include "list.h"
#include "movie.h"

void Interface__read_movie(BPTree *tree, List **list);
void Interface__print_movie(Movie *movie);
int Interface__menu();
void Interface__movie_search(BPTree *tree, List *list);
void Interface__movie_search_key(BPTree *tree);
void Interface__movie_search_title(List *list);
void Interface__update_score();
void Interface__movie_list(BPTree *tree);
void Interface__movie_list_range(BPTree *tree);
void Interface__movie_list_all(BPTree *tree);

#endif // !INTERFACE_H
