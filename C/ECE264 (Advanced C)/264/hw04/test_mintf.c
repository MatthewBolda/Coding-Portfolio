#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "mintf.h"
#include <limits.h>

int main(int argc, char*argv[]) {
	
	//test of number as a string
	mintf("1234");
	fputc('\n', stdout);
	//test of %d
	mintf("%d", 1234);
	fputc('\n', stdout);
	//test of string followed by %d
	mintf("The largest number I can think of is %d, I'm just kidding",50);
	fputc('\n', stdout);
	//test of multiple %d	
	mintf("The loneliest number is the number %d followed by the number %d",1,2);
	fputc('\n', stdout);
	//test of %b
	mintf("%b", 100);
	fputc('\n', stdout);
	//test of string followed by %b
	mintf("The number 50 in binary is %b",50);
	fputc('\n', stdout);
	//test of multiple %b
	mintf("Both %b and %b are numbers in binary",10,20);
	fputc('\n', stdout);
	//test of %x
	mintf("%x", 170);
	fputc('\n', stdout);
	//test of string followed by %x
	mintf("The number 50 in hex is %x",50);
	fputc('\n', stdout);
	//test of multiple %x	
	mintf("Both %x and %x are numbers in hex",10,3500);
	fputc('\n', stdout);
	//test of single %
	mintf("This is a test of %");
	fputc('\n', stdout);
	//test of double %%
	mintf("This is a test of double %% I hope it works");
	fputc('\n', stdout);
	//test of invalid parameter
	mintf("The parameter %m is not a thing, but its my favorite letter");
	fputc('\n', stdout);
	//test of %s
	mintf("%s", "word");
	fputc('\n', stdout);
	//test of string followed by %s
	mintf("The number 50 as a string is %s","50");
	fputc('\n', stdout);
	//test of multiple %s
	mintf("Both %s and %s are numbers as strings","10","20");
	fputc('\n', stdout);
	//test of ASCII character
	mintf("%c", 75);
	fputc('\n', stdout);
	//test %$
	mintf("%$", 12.3456);
	fputc('\n', stdout);

	

	return EXIT_SUCCESS;
}

