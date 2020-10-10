#include "bmp.h"
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	//START TESTING READ_BMP WITH VALID HEADER
	const char* error = NULL;
	FILE* fp = fopen("green_earth.bmp", "r");
	BMPImage* image = read_bmp(fp, &error);
	if(image == NULL){
		fprintf(stderr, error);			
	}
	else{
		//free_bmp(image);
	}
	//end testing read_bmp but havent free because still need to test write
	fclose(fp);
	//testing read_bmp and write_bmp
	FILE* open_file = fopen("bmp_write_tester.bmp", "w");
	bool bmp_read_test = write_bmp(open_file, image, &error);
	if(bmp_read_test){
		printf("write_bmp successfully copied car.bmp\n");
	}
	else{
		fprintf(stderr, error);
	}

	free_bmp(image);
	fclose(open_file);
	// END OF TESTING READ_BMP AND WRITE_BMP
	

	// START TEST INVALID HEADER HERE
	
	fp = fopen("corrupt_carw.bmp", "r");
	BMPImage* corrupt_image = read_bmp(fp, &error);
	if(corrupt_image == NULL){
		fprintf(stderr, error);
	}
	fclose(fp);
	//free_bmp(corrupt_image);
	
	// END TEST OF INVALID HEADER HERE
	

	// BEGIN TESTING CROP
	fp = fopen("6x6_24bit.bmp", "r");
	BMPImage* uncropped_image = read_bmp(fp, &error);
	BMPImage* cropped_image = crop_bmp(uncropped_image, 5, 5, 5, 5, &error);
	FILE* file_for_crop_test = fopen("crop_tester.bmp", "w");
	bool crop_bmp_test = write_bmp(file_for_crop_test, cropped_image, &error);
	if(!crop_bmp_test){
		fprintf(stderr, error);
	}
	free_bmp(uncropped_image);
	free_bmp(cropped_image);
	fclose(fp);
	fclose(file_for_crop_test);
	// END TESTING CROP


	return EXIT_SUCCESS;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
