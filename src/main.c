#include <stdio.h>
#include <stdlib.h>
#include "../include/movie.h"

int main(int argc, char *argv[])
{
	Movie *filme = Movie__create("Toy Story");

	printf("%s\n", Movie__getTitle(filme));

	Movie__destroy(filme);

	return EXIT_SUCCESS;
}
