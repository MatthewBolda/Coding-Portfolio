#include <stdio.h>
#include <stdlib.h>
#include "miniunit.h"

int test_plus_3();
/*
int test_minus_3();
int test_times_3();
int test_divided_3();
int times_3(int n){
int divided_3(int n){
int plus_3(int n){
int minus_3(int n){
*/
/*
int times_3(int n){
	return n*3;
}
int divided_3(int n){
	return n % 3;
}
*/
int plus_3(int n){
	return n * 3;
}
/*
int minus_3(int n){
	return n-3;
}
*/

int test_plus_3(){
	mu_start();
	mu_check(plus_3(1) == 4); 
	mu_end();
}
/*
int test_minus_3(){
	mu_start();
	mu_check(minus_3(1) == -2);
	mu_end();
}

int test_times_3(){
	mu_start();
	mu_check(times_3(1) == 3);
	mu_end();
}
int test_divided_3(){
	mu_start();
	mu_check(plus_3(1) == 0); //will fail
	mu_end();
}

*/
int main(int argc, char* argv[]) {
	mu_run(test_plus_3);
//	mu_run(test_minus_3);
//	mu_run(test_times_3);
//	mu_run(test_divided_3);
	return EXIT_SUCCESS;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
