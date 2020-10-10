/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
#include "smintf.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	//test basic string
	
	char* s = smintf("123456");
	printf("%s", s);
	free(s);
	fputc('\n', stdout);
	//test basic integer
	s = smintf("%d",123456);
	printf("%s", s);
	free(s);
	fputc('\n', stdout);
	//test multiple integers
	s = smintf("%d and %d are both small numbers", 1, 2);
	printf("%s", s);
	free(s);
	fputc('\n', stdout);
	//test multiple paramters of different kind
	s = smintf("%d and %b and %x are all numbers in different bases also %s is my name", 16, 16, 16, "Matt");
	printf("%s", s);
	free(s);
	fputc('\n', stdout);
	//test invalid parameter and double %%
	s = smintf("This is a test of double %% and invalid %m I hope it works");
	printf("%s", s);
	free(s);
	fputc('\n', stdout);
	// test of multiple hexs	
	s = smintf("%x %x", 16, 16);
	printf("%s", s);
	free(s);
	fputc('\n', stdout);
	//test of ascii
	s = smintf("%c", 75);
	printf("%s", s);
	free(s);
	fputc('\n', stdout);
	//test of string
	s = smintf("%s", "My name is Matt");
	printf("%s", s);
	free(s);
	fputc('\n', stdout);
	//test of dollar
	s = smintf("%$", 12.3456);
	printf("%s", s);
	free(s);
	fputc('\n', stdout);
	//test of negative
	s = smintf("%$", -12.3456);
	printf("%s", s);
	free(s);
	fputc('\n', stdout);
	//test of multiple negatives
	s = smintf("%d and %b and %x are all numbers in different bases also %s is my name", -16, -16, -16, "Matt");
	printf("%s", s);
	free(s);
	fputc('\n', stdout);
	
	return EXIT_SUCCESS;
}
