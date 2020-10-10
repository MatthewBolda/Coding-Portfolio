
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef struct { int x; int y; } Point;
char* read_file(const char* path, const char** a_error);
void write_file(const char* path, const char* contents, const char** a_error);
void write_point(const char* path, Point p, const char** a_error);
Point read_point(const char* path, Point p, const char** a_error);

int main(int argc, char* argv[]) {
	
	const char* error = NULL;
	char* string = read_file("abc.txt", &error);
	//printf("%s", string);
	if( error != NULL){
		
	}
	else{
		free(string);
	}

	write_file("empty.txt", "this is not an empty file anymore", &error);
	string = read_file("empty.txt", &error);
	if( error != NULL){
		
	}
	else{
		free(string);
	}
	//char* assert_string = "this is not an empty file anymore\n";
	//assert(strcmp(string, assert_string));
	//free(string);

	Point p1 = {.x = 1, .y = 1};
	write_point("point.txt", p1, &error);

	Point p2 = read_point("point.txt", p1, &error);
	assert( p1.x == p2.x && p1.y == p2.y);
	

	return EXIT_SUCCESS;
}

//read contents of file and return string
char* read_file(const char* path, const char** a_error){
	FILE* fp = fopen(path, "r");
	if(fp == NULL){
		*a_error = strerror(errno);
		return "error";
	}

	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	char* return_string = malloc( length * sizeof(*return_string));
	fread(return_string, sizeof(*return_string), length, fp);
	if(ferror(fp)){
		*a_error = strerror(errno);
		return return_string;
	}

	fclose(fp);
	return return_string;
}

//write given content to a file at specified path
void write_file(const char* path, const char* contents, const char** a_error){
	FILE* fp = fopen(path, "w");
	if(fp == NULL){
		*a_error = strerror(errno);
		return;
	}
	int length = strlen(contents);
	fwrite(contents, sizeof(*contents), length, fp);
	if(ferror(fp)){
		*a_error = strerror(errno);
		return;
	}
	fclose(fp);
}

//write a single Point to file at specified path
void write_point(const char* path, Point p, const char** a_error){
	FILE* fp = fopen(path, "w");
	fwrite(&p, sizeof(p), 1, fp);
	fclose(fp);
}


// read a point from the file at the specified path into a Point on the stack
Point read_point(const char* path, Point p, const char** a_error){
	Point return_point;
	FILE* fp = fopen(path, "r");
	
	fread(&return_point, sizeof(return_point), 1, fp);
	
	fclose(fp);
	return p;
}

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
