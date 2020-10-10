#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "miniunit.h"
#include "bmp.h"
#include "mtat.h"

int test_r_1();
int test_r_2();
int test_r_3();
int test_r_4();
int test_r_5();
//int test_threads_2();
int test_large_image_r_05();
int test_large_image_r_10();
int test_large_image_r_20();
int test_large_image_r_40();
int test_empty_image();


int test_r_1(){
	mu_start();
	FILE* fp = fopen("img1_6x6_color.bmp", "r");
	const char* error = NULL;
	BMPImage* original_6x6 = read_bmp(fp, &error);
	fclose(fp);
	BMPImage* mono_6x6 = binarize(original_6x6, 1, 36, &error);
	fp = fopen("mono_6x6_r_1.bmp", "w");
	write_bmp(fp, mono_6x6, &error);
	free_bmp(original_6x6);
	free_bmp(mono_6x6);
	mu_check(fp != NULL);
	fclose(fp);
	//mu_check(fp == NULL);
	mu_end();
}
int test_threads_2(){
	mu_start();
	FILE* fp = fopen("img1_6x6_color.bmp", "r");
	const char* error = NULL;
	BMPImage* original_6x6 = read_bmp(fp, &error);
	fclose(fp);
	BMPImage* mono_6x6 = binarize(original_6x6, 1, 10, &error);
	fp = fopen("mono_6x6_r_1.bmp", "w");
	write_bmp(fp, mono_6x6, &error);
	free_bmp(original_6x6);
	free_bmp(mono_6x6);
	mu_check(fp != NULL);
	fclose(fp);
	//mu_check(fp == NULL);
	mu_end();
}
int test_empty_image(){
	mu_start();
	BMPImage* original_6x6 = NULL;
	const char* error = NULL;
	BMPImage* mono_6x6 = binarize(original_6x6, 1, 10, &error);
	mu_check( mono_6x6 == NULL);	
	//FILE* fp = fopen("mono_6x6_r_1.bmp", "w");
	//write_bmp(fp, mono_6x6, &error);
	//mu_check(fp != NULL);
	//fclose(fp);
	//mu_check(fp == NULL);
	mu_end();
}

int test_r_2(){
	mu_start();
	FILE* fp = fopen("img1_6x6_color.bmp", "r");
	const char* error = NULL;
	BMPImage* original_6x6 = read_bmp(fp, &error);
	fclose(fp);
	BMPImage* mono_6x6 = binarize(original_6x6, 2, 37, &error);
	fp = fopen("mono_6x6_r_2.bmp", "w");
	write_bmp(fp, mono_6x6, &error);
	free_bmp(original_6x6);
	free_bmp(mono_6x6);
	mu_check(fp != NULL);
	fclose(fp);
	//mu_check(fp == NULL);
	mu_end();
}
int test_r_3(){
	mu_start();
	FILE* fp = fopen("img1_6x6_color.bmp", "r");
	const char* error = NULL;
	BMPImage* original_6x6 = read_bmp(fp, &error);
	fclose(fp);
	BMPImage* mono_6x6 = binarize(original_6x6, 3, 7, &error);
	fp = fopen("mono_6x6_r_3.bmp", "w");
	write_bmp(fp, mono_6x6, &error);
	free_bmp(original_6x6);
	free_bmp(mono_6x6);
	mu_check(fp != NULL);
	fclose(fp);
	//mu_check(fp == NULL);
	mu_end();
}
int test_r_4(){
	mu_start();
	FILE* fp = fopen("img1_6x6_color.bmp", "r");
	const char* error = NULL;
	BMPImage* original_6x6 = read_bmp(fp, &error);
	fclose(fp);
	BMPImage* mono_6x6 = binarize(original_6x6, 4, 5, &error);
	fp = fopen("mono_6x6_r_4.bmp", "w");
	write_bmp(fp, mono_6x6, &error);
	free_bmp(original_6x6);
	free_bmp(mono_6x6);
	mu_check(fp != NULL);
	fclose(fp);
	//mu_check(fp == NULL);
	mu_end();
}
int test_r_5(){
	mu_start();
	FILE* fp = fopen("img1_6x6_color.bmp", "r");
	const char* error = NULL;
	BMPImage* original_6x6 = read_bmp(fp, &error);
	fclose(fp);
	BMPImage* mono_6x6 = binarize(original_6x6, 5, 11, &error);
	fp = fopen("mono_6x6_r_5.bmp", "w");
	write_bmp(fp, mono_6x6, &error);
	free_bmp(original_6x6);
	free_bmp(mono_6x6);
	mu_check(fp != NULL);
	fclose(fp);
	//mu_check(fp == NULL);
	mu_end();
}
int test_large_image_r_02(){
	mu_start();
	FILE* fp = fopen("img2_384x510_gray.bmp", "r");
	const char* error = NULL;
	BMPImage* original_large = read_bmp(fp, &error);
	fclose(fp);
	BMPImage* mono_large = binarize(original_large, 2, 800, &error);
	fp = fopen("mono_large_r_02.bmp", "w");
	write_bmp(fp, mono_large, &error);
	free_bmp(original_large);
	free_bmp(mono_large);
	mu_check(fp != NULL);
	fclose(fp);
	//mu_check(fp == NULL);
	mu_end();
}
int test_large_image_r_05(){
	mu_start();
	FILE* fp = fopen("img2_384x510_gray.bmp", "r");
	const char* error = NULL;
	BMPImage* original_large = read_bmp(fp, &error);
	fclose(fp);
	BMPImage* mono_large = binarize(original_large, 5, 10, &error);
	fp = fopen("mono_large_r_05.bmp", "w");
	write_bmp(fp, mono_large, &error);
	free_bmp(original_large);
	free_bmp(mono_large);
	mu_check(fp != NULL);
	fclose(fp);
	//mu_check(fp == NULL);
	mu_end();
}
int test_large_image_r_10(){
	mu_start();
	FILE* fp = fopen("img2_384x510_gray.bmp", "r");
	const char* error = NULL;
	BMPImage* original_large = read_bmp(fp, &error);
	fclose(fp);
	BMPImage* mono_large = binarize(original_large, 10, 10, &error);
	fp = fopen("mono_large_r_10.bmp", "w");
	write_bmp(fp, mono_large, &error);
	free_bmp(original_large);
	free_bmp(mono_large);
	mu_check(fp != NULL);
	fclose(fp);
	//mu_check(fp == NULL);
	mu_end();
}
int test_large_image_r_20(){
	mu_start();
	FILE* fp = fopen("img2_384x510_gray.bmp", "r");
	const char* error = NULL;
	BMPImage* original_large = read_bmp(fp, &error);
	fclose(fp);
	BMPImage* mono_large = binarize(original_large, 20, 30, &error);
	fp = fopen("mono_large_r_20.bmp", "w");
	write_bmp(fp, mono_large, &error);
	free_bmp(original_large);
	free_bmp(mono_large);
	mu_check(fp != NULL);
	fclose(fp);
	//mu_check(fp == NULL);
	mu_end();
}
int test_large_image_r_40(){
	mu_start();
	FILE* fp = fopen("img2_384x510_gray.bmp", "r");
	const char* error = NULL;
	BMPImage* original_large = read_bmp(fp, &error);
	fclose(fp);
	BMPImage* mono_large = binarize(original_large, 40, 30, &error);
	fp = fopen("mono_large_r_40.bmp", "w");
	write_bmp(fp, mono_large, &error);
	free_bmp(original_large);
	free_bmp(mono_large);
	mu_check(fp != NULL);
	fclose(fp);
	//mu_check(fp == NULL);
	mu_end();
}
int main(int argc, char* argv[]) {
	//int test_1 = test_r_1();
	//if(test_1){ printf("test_1 completed"); }
	mu_run(test_r_1);
	mu_run(test_empty_image);
	//mu_run(test_threads_2);
	
	mu_run(test_r_2);
	mu_run(test_r_3);
	mu_run(test_r_4);
	mu_run(test_r_5);
	mu_run(test_large_image_r_02);
	
	mu_run(test_large_image_r_05);
	
	mu_run(test_large_image_r_10);
	mu_run(test_large_image_r_20);
	mu_run(test_large_image_r_40);
	return EXIT_SUCCESS;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
