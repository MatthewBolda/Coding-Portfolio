#include <stdio.h>
#include <stdlib.h>

void print_contents_of_file(char* filename);
void print_first_n_chars_in_file(char* filename, int n);
char* get_first_line_of_file(char* filename);

int main(int argc, char* argv[]) {
//	print_contents_of_file("abc.txt");
//	print_first_n_chars_in_file("abc.txt",5);
	char* string = get_first_line_of_file("abc.txt");
	printf("%s", string);
	free(string);


	return EXIT_SUCCESS;
}

//no malloc call in this fuction
void print_contents_of_file(char* filename){
	FILE* fp = fopen(filename, "r");
	char ch =  fgetc(fp);

	while(!feof(fp)){
		fputc(ch, stdout);
		ch = fgetc(fp);
	}
	fclose(fp);
}

//no malloc call in this fuction
void print_first_n_chars_in_file(char* filename, int n){
	FILE* fp = fopen(filename, "r");
	char ch = fgetc(fp);
	fputc(ch, stdout);
	
	int counter = 0;

	while(!feof(fp)){
		ch = fgetc(fp);
		fputc(ch, stdout);
		counter++;

		if (counter == (n-1)){
			break;
		}
	}
	
	
	fclose(fp);

}

//malloc should be called here
char* get_first_line_of_file(char* filename){
	FILE* fp = fopen(filename, "r");

	int counter = 1;
	char ch = fgetc(fp);

	while(!feof(fp)){
		if(ch == '\n'){
			break;
		}
		ch = fgetc(fp);
		counter ++;
	}
	char* string = malloc( (counter + 1) * sizeof(*string));
	fseek(fp, 0, SEEK_SET);
	
	for(int i = 0; i < counter; i++){
		string[i] = fgetc(fp);
	}
	string[counter] = '\0';
	fclose(fp);

	return string;


}

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
