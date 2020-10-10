/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
#include "bmp.h"
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

const int _BITS_PER_BYTE = 8;

int _size_in_bytes(const BMPHeader* bmp_header);

BMPImage* read_bmp(FILE* fp, const char** a_error){
	//check if the file is empty
	fseek(fp, 0, SEEK_END);
	if(ftell(fp) == 0){
		*a_error = "Error: Empty file";
		return NULL;
	}
	
	//malloc for the image, read header, error check both
	BMPImage* img = malloc(sizeof(*img));
	*a_error = (img != NULL ? NULL : strerror(errno));
	fseek(fp, 0, SEEK_SET);
	size_t read1 = fread( &img -> header, sizeof(img -> header), 1, fp);
	*a_error = (read1 != 1 ? NULL: strerror(errno));
	
	// check if header is valid
	bool is_header_safe = check_bmp_header(&img -> header, fp);
	if(!is_header_safe){
		*a_error = "Error: The BMP header is not valid";
		img -> data = NULL;
		free_bmp(img);
		return NULL;
	}
	
	//malloc data, read data from file, error check
	img -> data = malloc(img -> header.image_size_bytes * sizeof(*(img -> data)));
	*a_error = (img -> data != NULL ? NULL : strerror(errno));
	fseek(fp, sizeof(img -> header), SEEK_SET);
	size_t read2 = fread(img -> data, img -> header.image_size_bytes, 1, fp);
	*a_error = (read2 == 1 ? NULL: strerror(errno));
	
	return img;
}

int _size_in_bytes(const BMPHeader* bmp_header){
	int bytes_per_pixel = bmp_header -> bits_per_pixel / _BITS_PER_BYTE ;
	int num_pixels = bmp_header -> height_px * bmp_header -> width_px * bytes_per_pixel;
	int bytes_per_row_not_padding = bytes_per_pixel * bmp_header -> width_px;
	int padding_per_row = (4 - bytes_per_row_not_padding % 4) % 4;
	int padding_help = padding_per_row * bmp_header -> height_px;
	int image_size = num_pixels + padding_help;

	return image_size;
}

bool check_bmp_header(const BMPHeader* bmp_header, FILE* fp){
	//initialize constants here for easy changing
	uint16_t magic_number    = 0x4d42;
	uint16_t bits_per_pixel  = 0x18;
	uint16_t num_image_plane = 1;
	
	fseek(fp, 0, SEEK_END);
	uint32_t size = ftell(fp);
	uint32_t image_size_bytes = _size_in_bytes(bmp_header);
	
	//check requirements for valid bmp header
	bool is_valid = (bmp_header -> type == magic_number)
				 && (bmp_header -> offset == sizeof(*bmp_header))
				 && (bmp_header -> dib_header_size == DIB_HEADER_SIZE)
				 && (bmp_header -> num_planes == num_image_plane)
				 && (bmp_header -> compression == 0)
				 && (bmp_header -> num_colors == 0)
				 && (bmp_header -> important_colors == 0)
				 && (bmp_header -> bits_per_pixel == bits_per_pixel)
				 && (bmp_header -> image_size_bytes == image_size_bytes)
				 && (bmp_header -> size == size);

	return is_valid;
}

void free_bmp(BMPImage *image){
	if(image != NULL){
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
	BMPImage* crop_img = malloc(sizeof(*crop_img));
	*a_error = (crop_img != NULL ? NULL : strerror(errno));
	
	// change header to requirements for crop_img
	crop_img -> header = image -> header;
	crop_img -> header.width_px = w;
	crop_img -> header.height_px = h;
	uint32_t image_size_bytes = _size_in_bytes(&crop_img -> header);	
	crop_img -> header.image_size_bytes = image_size_bytes;
	crop_img -> header.size = image_size_bytes + BMP_HEADER_SIZE;
	crop_img -> data = malloc(image_size_bytes * sizeof(*(crop_img -> data)));
	
	//calculate old and new padding per row, as well as bytes per row
	int bytes_per_pixel = crop_img -> header.bits_per_pixel / _BITS_PER_BYTE;
	int crop_pixels_per_row = crop_img -> header.width_px * bytes_per_pixel;
	int crop_padding_per_row = ((4 - (crop_pixels_per_row) % 4) % 4);
	int original_pixels_per_row = image -> header.width_px * bytes_per_pixel;
	int original_padding_per_row = ((4 - (original_pixels_per_row) % 4) % 4);
	int bytes_per_row = bytes_per_pixel * image -> header.width_px + original_padding_per_row; 
	
	//initialize crop idx as well as find start idx for original image
	uint8_t idx = 0;
	int start_y = image -> header.height_px - h;
	int original_idx = x * bytes_per_pixel + ( start_y * bytes_per_row);

	for(int curr_y = 0; curr_y < h; curr_y++){
		for(int curr_x = 0; curr_x < w; curr_x++){
			for(int l = 0; l < bytes_per_pixel; l++){
				crop_img -> data[idx++] = image -> data[ original_idx++];
			}
		}
		for(int k = 0; k < crop_padding_per_row; k++){
			crop_img -> data[idx++] = 0;
		}
		start_y ++;
		original_idx = x * bytes_per_pixel + ( start_y * bytes_per_row);
	}

	return crop_img;
}
