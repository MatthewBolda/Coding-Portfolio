#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "mintf.h"
#include <stdbool.h>
#include <limits.h>



void mintf(const char *format, ...){
	//initialize the va function
	va_list more_args;
	va_start(more_args, format);
	//same concept as prefix from hw02 to search through string
	int idx_count = 0;
	int int_for_print = 0;
	char *string;
	int ascii_val;
	double double_for_print = 0;
	while(format[idx_count] != '\0'){
		// test to see if a parameter is potentially coming up
		if( format[idx_count] == '%'){
			switch (format[idx_count + 1]) {
				case 'd':									// for decimal
					int_for_print = va_arg(more_args, int);
					print_integer(int_for_print, 10, "");
					idx_count += 1;

					break;
				case 'b':									// for binary
					int_for_print = va_arg(more_args, int);
					print_integer(int_for_print, 2, "0b");
					idx_count += 1;

					break;
				case 'x':									// for hex
					int_for_print = va_arg(more_args, int);
					print_integer(int_for_print, 16, "0x");
					idx_count += 1;

					break;
				case '$':									// for double with prefix $
					double_for_print = va_arg(more_args, double);

					long long n = double_for_print * 100;				
					unsigned long long dollar_abs = n;
					// deals with printing a negative number
					if( n < 0 ) {
						fputc('-', stdout);
						dollar_abs = -n;
					}

					fputc('$', stdout);
					// helps find the length 
					int power_count = 0;
					unsigned long long dollar_power_count = dollar_abs;
					// finds power of given integer in given radix
					do {
						power_count = power_count + 1;
						dollar_power_count = dollar_power_count / 10;
					} while(dollar_power_count > 0);
					// fixes problem that counter counts 1 to high
					power_count -= 1;

					// Get the power value
					unsigned long long temporary_long = dollar_abs;
					int power_value = 1;
					for (int idx = 0; idx < power_count; idx++) {
						power_value *= 10;	
					}

					// print out the value using fputc
					for(int i = power_count; i >= 0; i--) {
						int digit = temporary_long / power_value;
						fputc('0' + digit, stdout);	
						if(i == 2){
							fputc('.', stdout);
						}
						temporary_long = temporary_long % (power_value);
						power_value = power_value / 10;
					}
					
					idx_count += 1;

					break;
				case 'c':									// for character literal
						
					ascii_val = va_arg(more_args, int);
					fputc(ascii_val, stdout);
					idx_count += 1;

					break;
				case 's':									// for string
					string = va_arg(more_args, char *);
					int string_idx_count = 0;
					while (string[string_idx_count] != '\0') {
						fputc(string[string_idx_count], stdout);
						string_idx_count +=1;
					}
					idx_count += 1;

					break;
				case '%':									// for putting %
					fputc('%', stdout);
					idx_count += 1;

					break;
				case '\0':									// testing for if % at end
					fputc('%', stdout);
					idx_count += 1;

					break;
				default:									// for everything else
					fputc('%', stdout);
					//idx_count += 1;

					break;
						

			}
		}

		else{
			fputc(format[idx_count], stdout);
		}
		idx_count += 1;
	}
	//end the va function
	va_end(more_args);

}





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
