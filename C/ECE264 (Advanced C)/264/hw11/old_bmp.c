#include "bmp.h"
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

//read_bmp error handling
//all of crop_bmp is needed
//error handling for write_bmp still needed

int _size_per_byte(const BMPHeader* bmp_header);

BMPImage* read_bmp(FILE* fp, const char** a_error){

	BMPImage* image_recieved = malloc(sizeof(*image_recieved));
	//if file isnt at start just send it to start
	fseek(fp, 0, SEEK_SET);
	size_t read1 = fread( &image_recieved -> header, sizeof(image_recieved -> header), 1, fp);
	// error testing for if it was able to read
	if(read1 != 1){
		*a_error = "The file could not be read";
		return NULL;
	}
	

	bool is_header_safe = check_bmp_header(&image_recieved -> header, fp);
	if(!is_header_safe){
		*a_error = "The BMP header is not valid";//change error message somehow
		image_recieved -> data = NULL;
		free_bmp(image_recieved);
		return NULL;
	}
	
	image_recieved -> data = malloc(image_recieved -> header.image_size_bytes * sizeof(*(image_recieved -> data)));
	fseek(fp, sizeof(image_recieved -> header), SEEK_SET);
	size_t read2 = fread(image_recieved -> data, image_recieved -> header.image_size_bytes, 1, fp);
	if(read2 != 1){
		*a_error = "The file could not be read";
		free_bmp(image_recieved);
		return NULL;
	}
	
	return image_recieved;
	
}

int _size_per_byte(const BMPHeader* bmp_header){
	int bits_per_byte = 8;
	int bytes_per_pixel = bmp_header -> bits_per_pixel / bits_per_byte;
	int padding_help = ((4 - (bmp_header -> width_px * (bytes_per_pixel)) % 4) % 4)* bmp_header -> height_px;
	
	int image_size = (bmp_header -> height_px * bmp_header -> width_px * bytes_per_pixel) + padding_help;
	
	return image_size;
}

bool check_bmp_header(const BMPHeader* bmp_header, FILE* fp){
	uint16_t magic_number    = 0x4d42;
	uint16_t bits_per_pixel  = 0x18;
	uint32_t dib_header_size = 40;
	uint16_t num_image_plane = 1;
	//bool is_valid_header = true;
	
	uint32_t size = 0;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	uint32_t image_size_bytes = _size_per_byte(bmp_header);
	
	//requirement 1
	bool check1 = (bmp_header -> type != magic_number) ? false: true;
	//is_valid_header = (bmp_header -> type != magic_number) ? false: true;
	//requirement 2
	bool check2 = (bmp_header -> offset != sizeof(*bmp_header)) ? false: true;
	//is_valid_header = (bmp_header -> offset != sizeof(*bmp_header)) ? false: true;
	//requirement 3
	bool check3 = (bmp_header -> dib_header_size != dib_header_size) ? false: true;
	//is_valid_header = (bmp_header -> dib_header_size != dib_header_size) ? false: true;
	//requirement 4
	bool check4 = (bmp_header -> num_planes != num_image_plane) ? false: true;
	//is_valid_header = (bmp_header -> num_planes != num_image_plane) ? false: true;
	//requirement 5
	bool check5 = (bmp_header -> compression != 0) ? false: true;
	//is_valid_header = (bmp_header -> compression != 0) ? false: true;
	//requirement 6
	bool check6 = ((bmp_header -> num_colors) != 0 || (bmp_header -> important_colors != 0)) ? false: true;
	//is_valid_header = ((bmp_header -> num_colors) != 0 || (bmp_header -> important_colors != 0)) ? false: true;
	//requirement 7
	bool check7 = (bmp_header -> bits_per_pixel != bits_per_pixel) ? false: true;
	//is_valid_header = (bmp_header -> bits_per_pixel != bits_per_pixel) ? false: true;
	//requirement 8
	bool check8 = ((bmp_header -> image_size_bytes != image_size_bytes) || (bmp_header -> size != size)) ? false: true;
	//is_valid_header = ((bmp_header -> image_size_bytes != image_size_bytes) || (bmp_header -> size != size)) ? false: true;
	bool checkall = (check1 && check2 && check3 && check4 && check5 && check6 && check7 && check8);

	return checkall;
	//return is_valid_header;
}

void free_bmp(BMPImage *image){
	//if(image != NULL){
	if(image -> data != NULL){
		free(image -> data);
	}
	free(image);
	//}
	//else{
	//	free(image);
	//}
}

bool write_bmp(FILE* fp, BMPImage* image, const char** a_error){
	//make sure file is at beginning, write header, write data
	fseek(fp, 0, SEEK_SET);
	size_t written1 = fwrite(&image -> header, sizeof(image -> header), 1, fp);
	// ERROR CHECK HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
	if(written1 != 1){
		*a_error = "The file could not be written";
		return false;
	}
	size_t written2 = fwrite(image -> data, image -> header.image_size_bytes, 1, fp);
	// ERROR CHECK HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
	if(written2 != 1){
		*a_error = "The file could not be written";
		return false;
	}
	
	return true;
}

BMPImage* crop_bmp(const BMPImage* image, int x, int y, int w, int h, const char** a_error){
	BMPImage* cropped_bmp = malloc(sizeof(*cropped_bmp));
	if(cropped_bmp == NULL){
		*a_error = "Malloc failed";
	}

	cropped_bmp -> header = image -> header;
	//update the new requirements for it to be cropped
	cropped_bmp -> header.width_px = w;
	cropped_bmp -> header.height_px = h;
	cropped_bmp -> header.image_size_bytes = _size_per_byte(&cropped_bmp -> header);
	cropped_bmp -> header.size = BMP_HEADER_SIZE + cropped_bmp -> header.image_size_bytes; 

	cropped_bmp -> data = malloc(cropped_bmp -> header.image_size_bytes * sizeof(*(cropped_bmp -> data)));
	
	int bits_per_byte = 8;
	int bytes_per_pixel = cropped_bmp ->header.bits_per_pixel / bits_per_byte;
	
	int position = 0;
	int index = 0;
	for(int i = 0; i< h; i++){
		for(int width = 0; width < ((w * (bytes_per_pixel))  + (w % 4)); width++){
			if(width >= w * (bytes_per_pixel)){
				cropped_bmp -> data[position] = 0;
			}
			cropped_bmp -> data[position] = image -> data[index + width];
			position = position + 1;
		}
		index = index + image ->header.width_px * (bytes_per_pixel);
	}
	return cropped_bmp;
}
