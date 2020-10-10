#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "backwards_file.h"

int _find_last_new_line(unsigned char* buffer, int length);
int _past_buffer_read_line(FileWrapper* fw, char** a_error);
int _find_last_new_line_without_whole_buffer(unsigned char* buffer, int length, bool* done);


FileWrapper* create_file_wrapper(char* filename, char** a_error){
	FILE* open_file = fopen(filename, "r");
	if(open_file == NULL){
		*a_error = strerror(errno);
	}
	FileWrapper* wrapper = malloc(sizeof(*wrapper));
	wrapper -> fp = open_file;
	if(ferror(wrapper -> fp)){
		*a_error = strerror(errno);
	}
	wrapper -> buffer_size = 1024;
	
	fseek(wrapper -> fp, 0, SEEK_END);
	int end_position = ftell(wrapper -> fp);
	wrapper -> end_position = end_position;

	wrapper -> buffer_address = malloc((wrapper -> buffer_size)*(sizeof(*(wrapper ->buffer_address)))); 

	return wrapper;

}

int _find_last_new_line(unsigned char* buffer, int length) {
	int count = 0;
	for(int i = (length -1) ; i >= 0; i--){
		if(buffer[i] == '\n' && i != (length-1)){
			return count;
		}
		count ++;
	}
	return count;
}
int _find_last_new_line_without_whole_buffer(unsigned char* buffer, int length, bool* done) {
	int count = 0;
	for(int i = (length -1) ; i >= 0; i--){
		if(buffer[i] == '\n' && i != (length-1)){
			*done = true;
			return count;
		}
		count ++;
	}
	*done = false;
	return count;
}
int _past_buffer_read_line(FileWrapper* fw, char** a_error){
	
	int counter = 0;
	int start;
	int position;
	if((fw -> end_position) < (fw -> buffer_size)){
		counter = fw -> end_position;
		start = 0;
		position = SEEK_SET;
	}
	else {
		counter = fw -> buffer_size;
		start = -(fw -> buffer_size);
		position = SEEK_CUR;
	}
	fseek(fw -> fp, start, position);
	fread(fw -> buffer_address, sizeof(*(fw -> buffer_address)), counter, fw -> fp);
	if(ferror(fw -> fp)){
		*a_error = strerror(errno);
	}
	fseek(fw -> fp, start, position);
	bool should_i_stop;
	int loc_in_buf = _find_last_new_line_without_whole_buffer(fw -> buffer_address, counter,&should_i_stop);
	if(should_i_stop){
		return loc_in_buf;
	}

	return loc_in_buf;
}

unsigned char* read_line(FileWrapper* fw, char** a_error){
	int counter = 0;
	int start;
	int position;
	bool end_of_file;
	if((fw -> end_position) < (fw -> buffer_size)){
		counter = fw -> end_position;
		start = 0;
		position = SEEK_SET;
		end_of_file = true;
	}
	else {
		counter = fw -> buffer_size;
		start = -(fw -> buffer_size);
		position = SEEK_CUR;
		end_of_file = false;
	}
	fseek(fw -> fp, start, position);
	fread(fw -> buffer_address, sizeof(*(fw -> buffer_address)), counter, fw -> fp);
	fseek(fw -> fp, start, position);

	int loc_in_buf = _find_last_new_line(fw -> buffer_address, counter);
	if(end_of_file){
		counter -= loc_in_buf;
		fseek(fw->fp, loc_in_buf, SEEK_CUR);
		
		unsigned char* return_string = malloc((loc_in_buf + 1)*sizeof(*return_string));
		for(int i = 0; i < loc_in_buf;i++) {
			return_string[i] = fw -> buffer_address[counter + i];			//fgetc(fw -> fp);
		}
		return_string[loc_in_buf] = '\0';
		
		fw -> end_position = (fw -> end_position) - loc_in_buf;	
		return return_string;
	}
	else{
		fw -> end_position = (fw -> end_position) - loc_in_buf;
		counter +=_past_buffer_read_line(fw,a_error);
		fseek(fw-> fp, -counter, SEEK_END);
		
		unsigned char* return_string = malloc((counter + 1)*sizeof(*return_string));
		char ch = fgetc(fw -> fp);
		return_string[0] = ch;
		for(int i = 1; i < counter;i++) {
			ch = fgetc(fw -> fp);
			return_string[i] = ch;						//fgetc(fw -> fp);
		}
		return_string[counter] = '\0';
		
		fw -> end_position = (fw -> end_position) - counter;	

		fseek(fw-> fp, -counter, SEEK_END);
		return return_string;
	}
}


void free_wrapper(FileWrapper* fw){

	fclose(fw -> fp);
	free(fw -> buffer_address);
	free(fw);

}


/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
