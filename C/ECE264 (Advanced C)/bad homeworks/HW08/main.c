#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pa08.h"


int main(int argc, char *argv[]) {

	char * word;
	DoubleVar * var = malloc(sizeof(DoubleVar));
	
	var -> digit_start = * argv[2];
	var -> digit_end = * argv[3];
	
	int length = atoi(argv[4]);

	for(int i = 3; i <= length; i++)
	{
		word = malloc(sizeof(char) * i);

		for(int j = 0; j < i; j++)
		{
			word[j] = (var -> digit_start);
		}

		createDouble(word, var, 0, false, i);
		free(word);
	}

	free(var);

	
	return EXIT_SUCCESS;
}
