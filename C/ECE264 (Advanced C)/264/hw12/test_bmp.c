#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "miniunit.h"
#include "bmp.h"
void set_pixel(BMPImage* img, size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b);
BMPImage* create_bmp(size_t w, size_t h, uint8_t r, uint8_t g, uint8_t b);
int test_read();
int test_write();
int test_failed_header();
int test_crop_no_padding();
int test_crop_padding();
int test_pixel();


int main(int argc, char* argv[]) {

	mu_run(test_write); 		//DONE	//i believe this test is complete, should pass
	mu_run(test_read);			//DONE	//i believe this test is complete, should pass
	mu_run(test_failed_header);	//DONE  //i believe this test is complete, should pass 
	mu_run(test_crop_no_padding); 					//i believe this test is complete, should pass
	mu_run(test_crop_padding); 					//i believe this test is complete, should pass
	mu_run(test_pixel);			//DONE	//i believe this test is complete, should pass

	return EXIT_SUCCESS;
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
	BMPImage* test_image = create_bmp(6,6,0xff,0xaa,0xaa);
	FILE* fp = fopen("test_write_read.bmp","w");
	const char* error = NULL;
	bool is_success = write_bmp(fp, test_image, &error);
	mu_check( is_success);
	fclose(fp);
	fp = fopen("test_write_read.bmp", "r");
	BMPImage* test_read_image = read_bmp(fp, &error);
	mu_check( test_read_image == test_image );	
	fclose(fp);
	mu_check( test_image != NULL);	
	free_bmp(test_image);
	mu_check( test_image == NULL);	
	mu_check( test_image != NULL);	
	free_bmp(test_read_image);
	mu_check( test_image == NULL);	
	mu_check(error == NULL);
	mu_end();
}
int test_crop_no_padding(){

	mu_start();																	
	BMPImage* test_image = create_bmp(6,6,0xff,0xaa,0xbb);
	FILE* fp = fopen("test_write_read.bmp","w");
	const char* error = NULL;
	bool is_success = write_bmp(fp, test_image, &error);
	mu_check( is_success);
	BMPImage* cropped_bmp = crop_bmp(test_image, 2, 3, 4, 2, &error);
	set_pixel(test_image, 2, 3, 0xcc,0xcc,0xcc);
	BMPImage* cropped_bmp2 = crop_bmp(test_image, 2, 3, 4, 2, &error);
	mu_check( test_image -> data != cropped_bmp -> data);
	mu_check( cropped_bmp2 -> data != cropped_bmp -> data);
	mu_check( check_bmp_header(&cropped_bmp -> header, fp));
	mu_check(cropped_bmp -> header.height_px < test_image -> header.height_px && cropped_bmp -> header.width_px < test_image -> header.width_px);
	mu_check(cropped_bmp -> header.size <= test_image -> header.size);
	free_bmp(test_image);
	free_bmp(cropped_bmp);
	free_bmp(cropped_bmp2);
	mu_check( test_image == NULL);	
	mu_check( cropped_bmp == NULL);	
	fclose(fp);
	mu_check(error == NULL);
	mu_end();
}
int test_crop_padding(){

	mu_start();																	
	BMPImage* test_image = create_bmp(6,6,0xff,0xaa,0xbb);
	FILE* fp = fopen("test_write_read.bmp","w");
	const char* error = NULL;
	bool is_success = write_bmp(fp, test_image, &error);
	mu_check( is_success);
	BMPImage* cropped_bmp = crop_bmp(test_image, 5, 5, 1, 1, &error);
	set_pixel(test_image, 1, 1, 0xcc,0xcc,0xcc);
	BMPImage* cropped_bmp2 = crop_bmp(test_image, 5, 5, 1, 1, &error);
	mu_check( test_image -> data != cropped_bmp -> data);
	mu_check( cropped_bmp2 -> data != cropped_bmp -> data);
	mu_check( check_bmp_header(&cropped_bmp -> header, fp));
	mu_check(cropped_bmp -> header.height_px < test_image -> header.height_px && cropped_bmp -> header.width_px < test_image -> header.width_px);
	mu_check(cropped_bmp -> header.size <= test_image -> header.size);
	free_bmp(test_image);
	free_bmp(cropped_bmp);
	free_bmp(cropped_bmp2);
	mu_check( test_image == NULL);	
	mu_check( cropped_bmp == NULL);	
	fclose(fp);
	mu_check(error == NULL);
	mu_end();
}
int test_failed_header(){

	mu_start();																	
	BMPImage* test_image = create_bmp(6,6,0xff,0xaa,0xaa);
	// making it corrupt to all every single header requirement
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

int test_pixel(){
	
	mu_start();																	
	BMPImage* test_image = create_bmp(6,6,0xff,0xaa,0xaa);
	BMPImage* compare_image = create_bmp(6,6,0xff,0xaa,0xaa);
	mu_check( compare_image -> data == test_image -> data);	
	set_pixel(test_image, 1, 1, 0x11,0x11,0x11);
	mu_check( compare_image -> data != test_image -> data);	
	FILE* fp = fopen("test_set_pixel.bmp", "w");
	const char* error = NULL;
	write_bmp(fp, test_image,&error);
	mu_check( test_image != NULL);	
	mu_check( compare_image != NULL);	
	free_bmp(test_image);
	free_bmp(compare_image);
	mu_check( test_image == NULL);	
	mu_check( compare_image == NULL);	
	fclose(fp);
	mu_check(error == NULL);
	mu_end();
	
}

void set_pixel(BMPImage* img, size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b){

	int bits_per_byte = 8;
	int bytes_per_pixel = img -> header.bits_per_pixel / bits_per_byte;
	int padding_per_row = ((4 - (img -> header.width_px * (bytes_per_pixel)) % 4) % 4);
	int bytes_per_row = bytes_per_pixel * img -> header.width_px + padding_per_row; 
	int start_y = img -> header.height_px - y -1 ;
	int idx = x * bytes_per_pixel + ( start_y * bytes_per_row);
	img -> data[idx++] = b;
	img -> data[idx++] = g;
	img -> data[idx++] = r;
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
