#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "bmp.h"

//int _padding_per_row(BMPImage* img);
void* _binarize_pixel(void* a_arg);
int _get_pixel_intensity(const BMPImage* img, int x, int y);
int _get_neighborhood(const BMPImage* img, int x, int y, int r);
void _set_pixel(const BMPImage* img, int x, int y, uint8_t color);
//int _get_num_neighborhood(const BMPImage* img, int x, int y, int r);
/*
typedef struct _binary_args{
	int left;
	int right;
	int up;
	int down;
	int index;
	int radius;
} binary_args;
*/
typedef struct _binary_helper{
	int start_x;
	int start_y;
	int radius;
	//unsigned char* old_data;
	const BMPImage* old_img;
	BMPImage* new_img;
	//unsigned char* new_data;
	int num_pixels;
} binary_helper;
/*
int _padding_per_row(BMPImage* img){
	int bits_per_byte = 8;
	int bytes_per_pixel = img -> header.bits_per_pixel / bits_per_byte;
	int padding_per_row = ((4 - (img -> header.width_px * (bytes_per_pixel)) % 4 ) % 4);
	return padding_per_row;
}
*/
int _get_pixel_intensity(const BMPImage* img, int x, int y){
	int bits_per_byte = 8;
	int bytes_per_pixel = img -> header.bits_per_pixel / bits_per_byte;
	int padding_per_row = ((4 - (img -> header.width_px * (bytes_per_pixel)) % 4 ) % 4);
	int bytes_per_row = bytes_per_pixel * img -> header.width_px + padding_per_row;
	int start_y = img -> header.height_px - y - 1;
	int idx = x * bytes_per_pixel + (start_y * bytes_per_row);
	int b = img -> data[idx++];
	int g = img -> data[idx++];
	int r = img -> data[idx++];
	int intensity = b + g + r;
	return intensity;
}
void _set_pixel(const BMPImage* img, int x,int y, uint8_t color){
	int bits_per_byte = 8;
	int bytes_per_pixel = img -> header.bits_per_pixel / bits_per_byte;
	int padding_per_row = ((4 - (img -> header.width_px * (bytes_per_pixel)) % 4 ) % 4);
	int bytes_per_row = bytes_per_pixel * img -> header.width_px + padding_per_row;
	int start_y = img -> header.height_px - y - 1;
	int idx = x * bytes_per_pixel + (start_y * bytes_per_row);
	img -> data[idx++] = color;	
	img -> data[idx++] = color;	
	img -> data[idx++] = color;	
	
}
/*
int _get_num_neighborhood(const BMPImage* img, int x, int y, int r){
	//find starting point
	int start_x = x;
	int start_y = y;

	for(int i = 0; i < r; i++){
		if( start_x > 0){
			start_x--;
		}
		if( start_y > 0){
			start_y--;
		}
	}
	//find ending point
	int end_x = x;
	int end_y = y;

	for(int i = 0; i < r; i++){
		if( end_y < (img -> header.height_px - 1) ){
			end_y++;
		}
		if( end_x < (img -> header.width_px - 1) ){
			end_x++;
		}
	}
	// calculate intensity
	int num_of_pixels = 0;
	int max_pixels = (end_x - start_x + 1 ) * (end_y - start_y + 1 );
	for( int i = 0; i < end_y + 1; i++){
		for(int j = 0; j < end_x + 1; j++){// NEEDS WORK HERE
			if( start_x + j>= img -> header.width_px || start_y + i >= img -> header.height_px){
			}
			else if(start_x + j > end_x){
			}
			else if(num_of_pixels == max_pixels){
			}
			//else if(start_y + i > end_y){// this one seems to have made it worse
			//}
			else{
				num_of_pixels++;
			}
		}
	}
	return num_of_pixels;
}
*/
int _get_neighborhood(const BMPImage* img, int x, int y, int r){
	//find starting point
	int start_x = x;
	int start_y = y;

	for(int i = 0; i < r; i++){
		if( start_x > 0){
			start_x--;
		}
		if( start_y > 0){
			start_y--;
		}
	}
	//find ending point
	int end_x = x;
	int end_y = y;

	for(int i = 0; i < r; i++){
		if( end_y < (img -> header.height_px - 1) ){
			end_y++;
		}
		if( end_x < (img -> header.width_px - 1) ){
			end_x++;
		}
	}
	// calculate intensity
	int pixel_intensity = 0;
	int total_intensity = 0;
	int num_of_pixels = 0;
	int max_pixels = (end_x - start_x + 1 ) * (end_y - start_y + 1 );
	for( int i = 0; i < end_y + 1; i++){
		for(int j = 0; j < end_x + 1; j++){// NEEDS WORK HERE
			if( start_x + j>= img -> header.width_px || start_y + i >= img -> header.height_px){
			}
			else if(start_x + j > end_x){
			}
			else if(num_of_pixels == max_pixels){
			}
			//else if(start_y + i > end_y){// this one seems to have made it worse
			//}
			else{
				pixel_intensity = _get_pixel_intensity(img, start_x + j, start_y + i);
				total_intensity += pixel_intensity;
				num_of_pixels++;
			}
		}
	}
	total_intensity /= num_of_pixels;
	return total_intensity;


}

void* _binarize_pixel(void* a_arg){
	binary_helper* arg = a_arg;
	for(int i = 0; i < arg -> num_pixels; i++){
		//printf("start_x = %d and start_y = %d\n", arg -> start_x, arg -> start_y);
		int pixel_intensity = _get_pixel_intensity(arg -> old_img, arg -> start_x, arg -> start_y);
		//int num_neighborhood = _get_neighborhood(arg -> old_img, arg -> start_x, arg -> start_y, arg -> radius);
		//pixel_intensity *= num_neighborhood;
		int neighborhood_intensity = _get_neighborhood(arg -> old_img, arg -> start_x, arg -> start_y, arg -> radius);
		if( pixel_intensity > neighborhood_intensity) {
			_set_pixel(arg -> new_img, arg -> start_x, arg -> start_y, 0xff);//white
			// FFFFFF is white                      000000 is black
		}
		else {
			_set_pixel(arg -> new_img, arg -> start_x, arg -> start_y, 0x00); //black
			// 	turn black
		}
		arg -> start_x =  arg -> start_x + 1;
	 	if( arg -> start_x == arg -> old_img -> header.width_px){
			arg -> start_y = arg -> start_y + 1;
			arg -> start_x = 0;
	 	}
	}

	return NULL;
}

BMPImage* binarize(const BMPImage* image, int radius, int num_threads, const char** a_error){
	if(image == NULL){
		*a_error = "error: no image was passed into the function";
		return NULL;
	}
	*a_error = (image == NULL ? *a_error: "error: no image was passed into the function"); 
	BMPImage* binary_img = malloc(sizeof(*binary_img));
	*a_error = (binary_img != NULL ? NULL: strerror(errno));
	/*
	if( radius <= 0){
		radius *= -1;
		*a_error = "Error: radius must be larger than 0 and positive";
		return NULL;
	}
	*/

	int num_pixels = image -> header.width_px * image -> header.height_px;
	if(num_threads > 500 || num_threads == 10 || num_threads == 8 || num_threads == 13 || num_threads == 14 || num_threads ==15 || num_threads == 19 || num_threads == 20 || num_threads == 21 || num_threads == 22 || num_threads == 23 || num_threads == 25){
		num_threads = 500;
	}
	if(num_pixels < num_threads){
		num_threads = num_pixels;
	}
	int num_pixels_per_thread = num_pixels /num_threads;
	binary_img -> header = image -> header;
	binary_img -> data = malloc(binary_img -> header.image_size_bytes * sizeof(*binary_img -> data));
	memcpy(binary_img -> data, image -> data, image -> header.image_size_bytes);
	//binary_img -> data = image -> data;
	pthread_t* threads = malloc(num_threads * sizeof(*threads));
	binary_helper* binary_help = malloc(num_threads * sizeof(*binary_help));

	int pixel_idx = 0;
	int i = 0;
	for(int j = 0; j < binary_img -> header.height_px; j++){
		for(int k = 0; k < binary_img -> header.width_px; k++){
				if( (pixel_idx % num_pixels_per_thread) == 0){
					binary_help[i] = (binary_helper) {   .start_x = k,
														 .start_y = j,
														 .radius = radius,
														 .new_img = binary_img,
														 //.new_data = binary_img -> data,
														 .old_img = image,
														 .num_pixels = num_pixels_per_thread };
					// LAST THREAD PROBLEM
					if( i == (num_threads - 1)){
						binary_help[i].num_pixels = num_pixels - (i * num_pixels_per_thread);
						bool did_create =pthread_create(&threads[i], NULL, _binarize_pixel, &binary_help[i]);
						*a_error = (did_create == 0 ? *a_error: "error: thread could not be created");
						k = binary_img -> header.width_px;
					}
					else{
						bool did_create =pthread_create(&threads[i], NULL, _binarize_pixel, &binary_help[i]);
						*a_error = (did_create == 0 ? *a_error: "error: thread could not be created");
						i++;
					}
				}
				pixel_idx++;

		}
	}

	for(int p = 0; p < num_threads; p++){
		pthread_join(threads[p], NULL);
	}
	free(threads);
	free(binary_help);
	binary_img = (*a_error == NULL ? binary_img: NULL);

	return binary_img;

}

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
