#ifndef __BACKREAD_H_20190325__
#define __BACKREAD_H_20190325__
#include <stdbool.h>
typedef struct {
	FILE* fp;
	int buffer_size;
	unsigned char* buffer_address;
	int end_position;
	// Complete the fields here. 
} FileWrapper;

FileWrapper* create_file_wrapper(char* fileName, char** a_error);
unsigned char* read_line(FileWrapper* fw, char** a_error);
void free_wrapper(FileWrapper* fw);
#endif
