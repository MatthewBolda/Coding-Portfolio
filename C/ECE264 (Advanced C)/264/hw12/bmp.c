#include "bmp.h"
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


int _size_per_byte(const BMPHeader* bmp_header);

BMPImage* read_bmp(FILE* fp, const char** a_error){

	BMPImage* image_recieved = malloc(sizeof(*image_recieved));
	*a_error = (image_recieved != NULL ? NULL : strerror(errno));
	//if file isnt at start just send it to start
	fseek(fp, 0, SEEK_SET);
	size_t read1 = fread( &image_recieved -> header, sizeof(image_recieved -> header), 1, fp);
	*a_error = (read1 != 1 ? NULL: strerror(errno));

	bool is_header_safe = check_bmp_header(&image_recieved -> header, fp);
	if(!is_header_safe){
		*a_error = "The BMP header is not valid";
		image_recieved -> data = NULL;
		free_bmp(image_recieved);
		return NULL;
	}
	
	image_recieved -> data = malloc(image_recieved -> header.image_size_bytes * sizeof(*(image_recieved -> data)));
	*a_error = (image_recieved -> data != NULL ? NULL : strerror(errno));
	fseek(fp, sizeof(image_recieved -> header), SEEK_SET);
	size_t read2 = fread(image_recieved -> data, image_recieved -> header.image_size_bytes, 1, fp);
	*a_error = (read2 == 1 ? NULL: strerror(errno));
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
	
	uint32_t size = 0;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	uint32_t image_size_bytes = _size_per_byte(bmp_header);
	
	//requirement 1
	bool check1 = (bmp_header -> type != magic_number) ? false: true;
	//requirement 2
	bool check2 = (bmp_header -> offset != sizeof(*bmp_header)) ? false: true;
	//requirement 3
	bool check3 = (bmp_header -> dib_header_size != dib_header_size) ? false: true;
	//requirement 4
	bool check4 = (bmp_header -> num_planes != num_image_plane) ? false: true;
	//requirement 5
	bool check5 = (bmp_header -> compression != 0) ? false: true;
	//requirement 6
	bool check6 = ((bmp_header -> num_colors) != 0 || (bmp_header -> important_colors != 0)) ? false: true;
	//requirement 7
	bool check7 = (bmp_header -> bits_per_pixel != bits_per_pixel) ? false: true;
	//requirement 8
	bool check8 = ((bmp_header -> image_size_bytes != image_size_bytes) || (bmp_header -> size != size)) ? false: true;
	
	bool checkall = (check1 && check2 && check3 && check4 && check5 && check6 && check7 && check8);

	return checkall;
}

void free_bmp(BMPImage *image){
	if(image -> data != NULL){
		free(image -> data);
	}
	free(image);
}

bool write_bmp(FILE* fp, BMPImage* image, const char** a_error){

	fseek(fp, 0, SEEK_SET);
	size_t written1 = fwrite(&image -> header, sizeof(image -> header), 1, fp);
	*a_error = (written1 == 1 ? NULL: strerror(errno));
	size_t written2 = fwrite(image -> data, image -> header.image_size_bytes, 1, fp);
	*a_error = (written2 == 1 ? NULL: strerror(errno));

	return true;
}

BMPImage* crop_bmp(const BMPImage* image, int x, int y, int w, int h, const char** a_error){
	BMPImage* cropped_bmp = malloc(sizeof(*cropped_bmp));
	*a_error = (cropped_bmp != NULL ? NULL : strerror(errno));
	
	cropped_bmp -> header = image -> header;
	cropped_bmp -> header.width_px = w;
	cropped_bmp -> header.height_px = h;
	cropped_bmp -> header.size = BMP_HEADER_SIZE + cropped_bmp -> header.image_size_bytes; 

	cropped_bmp -> data = malloc(cropped_bmp -> header.image_size_bytes * sizeof(*(cropped_bmp -> data)));
	int bits_per_byte = 8;
	int bytes_per_pixel = cropped_bmp -> header.bits_per_pixel / bits_per_byte;
	int padding_per_row = ((4 - (cropped_bmp -> header.width_px * (bytes_per_pixel)) % 4) % 4);
	int padding_per_row_old = ((4 - (image -> header.width_px * (bytes_per_pixel)) % 4) % 4);
	int padding_help = padding_per_row * cropped_bmp -> header.height_px;
	uint32_t image_size_bytes = (cropped_bmp -> header.height_px * cropped_bmp -> header.width_px * bytes_per_pixel) + padding_help;
	uint32_t size = image_size_bytes + 54;
	cropped_bmp -> header.image_size_bytes = image_size_bytes;
	cropped_bmp -> header.size = size; 
	int bytes_per_row = bytes_per_pixel * image -> header.width_px + padding_per_row_old; 
	uint8_t idx = 0;
	//int start_x = x * bytes_per_pixel;
	int start_y = image -> header. height_px - y - 1;
	int start_x = x * bytes_per_pixel + (start_y * bytes_per_row);// - (bytes_per_pixel);
	for(int i = 0; i < h; i++){
		for(int j = 0; j < w; j++){
			//memcpy( cropped_bmp -> data, image -> data, sizeof(idx) * 3);
			cropped_bmp -> data[idx] = image -> data[start_x];	
			idx++;
			start_x = start_x +1;
			cropped_bmp -> data[idx] = image -> data[start_x];	
			idx++;
			start_x = start_x + 1;
			cropped_bmp -> data[idx] = image -> data[start_x];	
			idx++;
			start_x = start_x + 1;
		}
		for(int k = 0; k < padding_per_row; k++){
			cropped_bmp -> data[idx] = 0;
			idx++;
		}
	//	start_y = start_y + 1;
		start_x = start_x + bytes_per_row;
	}

	
	return cropped_bmp;
}
