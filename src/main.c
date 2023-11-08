#include <stdio.h>
#include <stdlib.h>
#include "../include/movie.h"
#include "../include/buffer.h"

int main(int argc, char *argv[])
{
	Buffer *buff = Buffer__create();

	buff->append(buff, "Toy Story");
	buff->append(buff, "Idem");
	buff->append(buff, "The, Disney");
	buff->append(buff, "2005");
	buff->append(buff, "Estados Unidos");
	buff->append(buff, "8");

	printf("%s\n", buff->get_content(buff));

	Buffer__destroy(buff);

	return EXIT_SUCCESS;
}
