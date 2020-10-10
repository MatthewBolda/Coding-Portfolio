#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* _getLastLine(char* buff, int length);

int main(int argc, char* argv[]) {
	
	char* string = "abc\n123\ndef\n123";
	int length = strlen(string);
	char* print_string = _getLastLine(string, length);
	printf("%s", print_string);
	//printf("123");
	free(print_string);
	print_string = _getLastLine(string, length-4);
	printf("%s", print_string);
	free(print_string);
	print_string = _getLastLine(string, length-8);
	printf("%s", print_string);
	free(print_string);


	return EXIT_SUCCESS;
}

char* _getLastLine(char* buff, int length){
	int count = 0;
	for(int i = length; i > 0; i--){
		if((buff[i] == '\n') && (i != length)){
			break;
		}
		count ++;
	}
	count --;

	char* return_string = malloc((count + 1) * sizeof(*return_string));
	for(int i = 0; i <= count; i++){
		return_string[i] = buff[length - count + i];
		if(count == i){
			return_string[i] = '\0';
		}
	}
	return return_string;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
