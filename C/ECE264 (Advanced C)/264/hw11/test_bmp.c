#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "miniunit.h"
#include "bmp.h"
BMPImage* create_bmp(size_t w, size_t h, uint8_t r, uint8_t g, uint8_t b);
int test_read();
int test_write();
int test_failed_header();
int test_crop();
int test_pixel();
int test_empty_file();
int test_crop_full_image();

int main(int argc, char* argv[]) {
	
	mu_run(test_write);
	mu_run(test_read);
	mu_run(test_failed_header);
	mu_run(test_empty_file);
	mu_run(test_crop);
	mu_run(test_crop_full_image);
	return EXIT_SUCCESS;
}
int test_empty_file(){
	mu_start();
	FILE* fp = fopen("empty_file.bmp", "w");
	const char* error = NULL;
	assert(error == NULL);
	mu_check( error == NULL);
	read_bmp(fp, &error);
	fclose(fp);

	mu_check( error != NULL);
	mu_end();
}

int test_write(){
	mu_start();
	BMPImage* test_image = create_bmp(7,7,0xff,0xaa,0xaa);	
	FILE* fp = fopen("test_write.bmp", "w");
	const char* error = NULL;
	bool is_success = write_bmp(fp, test_image,&error);
	mu_check( is_success);
	mu_check( check_bmp_header(&test_image -> header, fp));
	mu_check( test_image != NULL);
	free_bmp(test_image);
	mu_check( test_image == NULL);	
	fclose(fp);
	mu_check(error == NULL);
	mu_end();
}

int test_read(){
	
	mu_start();																	
	const char* error = NULL;
	FILE* fp = fopen("6x6_24bit.bmp", "r");
	BMPImage* test_read_image = read_bmp(fp, &error);
	fclose(fp);
	mu_check( test_read_image != NULL);	
	free_bmp(test_read_image);
	mu_check( test_read_image == NULL);	
	mu_check(error == NULL);
	mu_end();
}
int test_crop(){

	mu_start();																	
	FILE* fp = fopen("6x6_24bit.bmp", "r");
	const char* error = NULL;
	BMPImage* image = read_bmp(fp, &error);
	BMPImage* crop = crop_bmp(image, 2, 0, 2, 2, &error);
	fclose(fp);
	fp = fopen("test_crop.bmp", "w");
	bool is_success = write_bmp(fp, crop, &error);
	mu_check(crop -> data[0] == image -> data [86]);
	mu_check(crop -> data[1] == image -> data [87]);
	mu_check(crop -> data[2] == image -> data [88]);
	mu_check(crop -> data[3] == image -> data [89]);
	mu_check(is_success);
	free_bmp(image);
	free_bmp(crop);
	mu_end();
}
int test_crop_full_image(){

	mu_start();																	
	FILE* fp = fopen("6x6_24bit.bmp", "r");
	const char* error = NULL;
	BMPImage* image = read_bmp(fp, &error);
	BMPImage* crop = crop_bmp(image, 0, 0, 6, 6, &error);
	fclose(fp);
	fp = fopen("test_crop.bmp", "w");
	bool is_success = write_bmp(fp, crop, &error);
	mu_check(crop -> data[22] == image -> data [22]);
	mu_check(crop -> data[60] == image -> data [60]);
	mu_check(crop -> data[80] == image -> data [80]);
	mu_check(crop -> data[118] == image -> data [118]);
	mu_check(is_success);
	free_bmp(image);
	free_bmp(crop);
	mu_end();
}
int test_failed_header(){

	mu_start();																	
	BMPImage* test_image = create_bmp(6,6,0xff,0xaa,0xaa);
	// making it corrupt to every single header requirement
	test_image -> header. type  = 0x4d40;
	test_image -> header. offset = 100;
	test_image -> header. dib_header_size = 50;
	test_image -> header. num_planes = 2;
	test_image -> header. compression = 1;
	test_image -> header. num_colors = 1;
	test_image -> header. bits_per_pixel  = 10;
	test_image -> header. width_px = 10;
	//now it will fail all 8 check_header_bmp requirement
	FILE* fp = fopen("test_write_read.bmp","w");
	const char* error = NULL;
	bool is_success = write_bmp(fp, test_image, &error);
	mu_check( is_success);
	mu_check( test_image -> data == NULL);	
	mu_check( test_image != NULL);	
	free_bmp(test_image);
	mu_check( test_image == NULL);	
	fclose(fp);
	fp = fopen("test_write_read.bmp", "r");
	BMPImage* test_read_image = read_bmp(fp, &error);
	mu_check( test_read_image == NULL);
	fclose(fp);
	mu_check(error != NULL); 
	mu_end();
	
}

BMPImage* create_bmp(size_t w, size_t h, uint8_t r, uint8_t g, uint8_t b){
	BMPImage* new_bmp = malloc(sizeof(*new_bmp));
	new_bmp -> header.type 				= 0x4d42;
	new_bmp -> header.reserved1 		= 0;
	new_bmp -> header.reserved2 		= 0;
	new_bmp -> header.offset 			= 0x36;
	new_bmp -> header.dib_header_size 	= 0x28;
	new_bmp -> header.width_px			= w;
	new_bmp -> header.height_px 		= h;
	new_bmp -> header.num_planes 		= 1;
	new_bmp -> header.bits_per_pixel 	= 0x18;
	new_bmp -> header.compression 		= 0;
	new_bmp -> header.x_resolution_ppm	= 0;
	new_bmp -> header.y_resolution_ppm	= 0;
	new_bmp -> header.num_colors  		= 0;
	new_bmp -> header.important_colors 	= 0;
	
	int bits_per_byte = 8;
	int bytes_per_pixel = new_bmp -> header.bits_per_pixel / bits_per_byte;
	int padding_per_row = ((4 - (new_bmp -> header.width_px * (bytes_per_pixel)) % 4) % 4);
	int padding_help = padding_per_row * new_bmp -> header.height_px;
	
	uint32_t image_size_bytes = (new_bmp -> header.height_px * new_bmp -> header.width_px * bytes_per_pixel) + padding_help;
	uint32_t size = image_size_bytes + 54;

	new_bmp -> header.size	 			= size;
	new_bmp -> header.image_size_bytes	= image_size_bytes;
	
	new_bmp -> data = malloc(new_bmp -> header.image_size_bytes * sizeof(*(new_bmp -> data)));
	uint8_t idx = 0;
	for(int i = 0; i < h; i++){
		for(int j = 0; j < w ; j++){
			new_bmp -> data[idx] = b;
			idx++;
			new_bmp -> data[idx] = g;
			idx++;
			new_bmp -> data[idx] = r;
			idx++;
		}
		for(int k = 0; k < padding_per_row; k++){
			new_bmp -> data[idx] = 0;
			idx++;
		}
	}
	return new_bmp;
}


/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
