#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <assert.h>
#include "backwards_file.h"


int main(int argc, char* argv[]) {
	char* error = NULL;
	FileWrapper* fw = create_file_wrapper("ab.txt",&error);

	unsigned char* line = read_line(fw, &error);
	printf("%s", line);
	free(line);

	line = read_line(fw, &error);
	printf("%s", line);
	free(line);
/*
	line = read_line(fw, &error);
	printf("%s", line);
	free(line);

	line = read_line(fw, &error);
	printf("%s", line);
	free(line);

	line = read_line(fw, &error);
	printf("%s", line);
	free(line);

	line = read_line(fw, &error);
	printf("%s", line);
	free(line);
*/
	free_wrapper(fw);

	return EXIT_SUCCESS;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
