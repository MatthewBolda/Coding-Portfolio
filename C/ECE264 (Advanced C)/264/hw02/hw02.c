#include <stdio.h>
#include "hw02.h"
#include <stdbool.h>

void print_integer(int n, int radix, char* prefix) {
	// to fix the INT_MIN
	unsigned int n_abs = n;
	// deals with printing a negative number
	if( n < 0 ) {
		fputc('-', stdout);
		n_abs = -n;
	}

	// deals with prefix
	int idx_count = 0;
	while(prefix[idx_count] != '\0') {
		fputc(prefix[idx_count], stdout);
		idx_count += 1;
	}

	// helps find the length 
	int power_count = 0;
	unsigned int original_integer_for_power_counter = n_abs;
	// finds power of given integer in given radix
	do {
		power_count = power_count + 1;
		original_integer_for_power_counter = original_integer_for_power_counter / radix;
	} while(original_integer_for_power_counter > 0);
	// fixes problem that counter counts 1 to high
	power_count -= 1;
	
	// Get the power value
	unsigned int temporary_integer = n_abs;
	int power_value = 1;
	for (int idx = 0; idx < power_count; idx++) {
		power_value *= radix;	
	}

	// print out the value using fputc
	for(int i = power_count; i >= 0; i--) {
		int digit = temporary_integer / power_value;
		if (digit > 9) {
			//digit += 'a' - ':';
			fputc('W' + digit, stdout);		
		}
		else {	
			fputc('0' + digit, stdout);		
		}
		temporary_integer = temporary_integer % (power_value);
		power_value = power_value / radix;
	}

}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
