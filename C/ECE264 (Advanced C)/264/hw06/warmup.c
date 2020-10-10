 /* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
#include <stdio.h>
#include <stdlib.h>

char* my_strdup(const char* original) {
	
	int character_counter = 1;
	for(int index = 0; original[index] != '\0'; index++){
		character_counter++;
	}

	char* string_copy = malloc(character_counter * sizeof(*string_copy));
	
	for(int index = 0; index != character_counter; index++){
		string_copy[index] = original[index];
	}
	return string_copy;	
}

int main(int argc, char *argv[]) {
	char s[] = "abc\n";
	fputs(s, stdout);  // Should print "abc" followed by a newline ('\n')

	char* t = my_strdup(s);
	fputs(t, stdout);  // Should print "abc" followed by a newline ('\n')
	free(t);

	return EXIT_SUCCESS;
}
