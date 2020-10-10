#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hw02.h"

int main(int argc, char* argv[]) {
	
	//test 0	
	print_integer(0, 10, "");
	fputc('\n', stdout);
	//test one integer
	print_integer(1, 10, "");
	fputc('\n', stdout);
	//test negative number	
	print_integer(-1000, 10, "");
	fputc('\n', stdout);
	//test binary number
	print_integer(10, 2, "");
	fputc('\n', stdout);
	//test random base number
	print_integer(32, 30, "");
	fputc('\n', stdout);
	//test random base number with letters instead of digits
	print_integer(100, 15, "");
	fputc('\n', stdout);
	//test random base number with letters instead of digits but very long
	print_integer(100000, 15, "");
	fputc('\n', stdout);
	//test prefix
	print_integer(1000, 10, "$");
	fputc('\n', stdout);
	//test negative prefix	
	print_integer(-1000, 10, "$");
	fputc('\n', stdout);
	//test large prefix
	print_integer(12, 10, "My Favorite Number is ");
	fputc('\n',stdout);
	//test int_max
	print_integer(INT_MAX, 10, "The maximum integer is ");
	fputc('\n',stdout);
	//test int_min
	print_integer(INT_MIN, 10, "");
	fputc('\n',stdout);


	return EXIT_SUCCESS;

}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
