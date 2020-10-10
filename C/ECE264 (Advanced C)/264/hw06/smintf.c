/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
#include "smintf.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

//declaring helper functions
int _character_counter(const char * format, va_list more_args); 
int _count_character_print_integer(int n, int radix, char* prefix);
void _string_initializing(const char * format, va_list more_args, char * string_print);
int _string_integer_initializing(int n, int radix, char* prefix,char * string_print, int index);

char* smintf(const char *format, ...) {
	va_list args_original, args_copy1, args_copy2;
	va_start(args_original, format);
	va_copy(args_copy1, args_original);
	va_copy(args_copy2, args_original);

	int character_count = _character_counter(format, args_copy1);
	char* return_string =  malloc( character_count * sizeof(*return_string));
	if(return_string == NULL) {
		return NULL;
	}
	_string_initializing(format, args_copy2, return_string);
	return_string[(character_count - 1)] = '\0';

	va_end(args_original);
	va_end(args_copy1);
	va_end(args_copy2);

	return return_string;
}
/*	
void mintf(const char *format, ...) {
	va_list more_args;
	va_start(more_args, format);
	va_copy(args_copy1, more_args);
	va_copy(args_copy2, more_args);
	
	int character_count = _character_counter(format, args_copy1 );
	char string_print[character_count] = _string_initializing(format, args_copy2, return_string);
	for(int i = 0; i <= character_count; i++) {
		//fputc(
	}
	va_end(more_args);
	va_end(args_copy1);
	va_end(args_copy2);
	
}
*/


//this is from HW04 modified to count characters instead of print
int _character_counter(const char * format, va_list more_args) {
	
	int character_count = 1;
	//same concept as prefix from hw02 to search through string
	int idx_count = 0;
	int int_for_print = 0;
	char *string;
	//int ascii_val = 0;
	double double_for_print = 0;
	while(format[idx_count] != '\0'){
		// test to see if a parameter is potentially coming up
		if( format[idx_count] == '%'){
			switch (format[idx_count + 1]) {
				case 'd':									// for decimal
					int_for_print = va_arg(more_args, int);
					character_count += _count_character_print_integer(int_for_print, 10, "");
					idx_count += 1;

					break;
				case 'b':									// for binary
					int_for_print = va_arg(more_args, int);
					character_count += _count_character_print_integer(int_for_print, 2, "0b");
					//character_count -= 1;
					idx_count += 1;

					break;
				case 'x':									// for hex
					int_for_print = va_arg(more_args, int);
					character_count += _count_character_print_integer(int_for_print, 16, "0x");
					//character_count -= 1;
					idx_count += 1;

					break;
				case '$':									// for double with prefix $
					double_for_print = va_arg(more_args, double);

					long long n = double_for_print * 100;				
					unsigned long long dollar_abs = n;
					// deals with printing a negative number
					if( n < 0 ) {
						character_count ++;
						//fputc('-', stdout);
						dollar_abs = -n;
					}
					
					character_count ++;
					//fputc('$', stdout);
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
						//int digit = temporary_long / power_value;
						character_count ++;
						//fputc('0' + digit, stdout);	
						if(i == 2){
							character_count ++;
							//fputc('.', stdout);
						}
						temporary_long = temporary_long % (power_value);
						power_value = power_value / 10;
					}
					
					idx_count += 1;

					break;
				case 'c':									// for character literal
						
					//ascii_val = va_arg(more_args, int);
					character_count ++;
					//fputc(ascii_val, stdout);
					idx_count += 1;

					break;
				case 's':									// for string
					string = va_arg(more_args, char *);
					int string_idx_count = 0;
					while (string[string_idx_count] != '\0') {
						character_count ++;
						//fputc(string[string_idx_count], stdout);
						string_idx_count +=1;
					}
					idx_count += 1;

					break;
				case '%':									// for putting %
					character_count ++;
					//fputc('%', stdout);
					idx_count += 1;

					break;
				case '\0':									// testing for if % at end
					character_count ++;
					//fputc('%', stdout);
					idx_count += 1;

					break;
				default:									// for everything else
					character_count ++;
					character_count ++;
					//fputc('%', stdout);
					idx_count += 1;

					break;
						

			}
		}

		else{
			character_count ++;
			//fputc(format[idx_count], stdout);
		}
		idx_count += 1;
	}

	return character_count;
}


//this is from HW04 modified to count characters instead of print
void _string_initializing(const char * format, va_list more_args, char* string_print) {
	
	int character_count = 0;
	//same concept as prefix from hw02 to search through string
	int idx_count = 0;
	int int_for_print = 0;
	char *string;
	int ascii_val = 0;
	double double_for_print = 0;
	while(format[idx_count] != '\0'){
		// test to see if a parameter is potentially coming up
		if( format[idx_count] == '%'){
			switch (format[idx_count + 1]) {
				case 'd':									// for decimal
						character_count ++;
					int_for_print = va_arg(more_args, int);
					character_count += _string_integer_initializing(int_for_print, 10, "", string_print, character_count);
					idx_count += 1;

					break;
				case 'b':									// for binary
					int_for_print = va_arg(more_args, int);
					character_count += _string_integer_initializing(int_for_print, 2, "0b", string_print, (character_count + 1));
					character_count += 1;
					idx_count += 1;

					break;
				case 'x':									// for hex
					int_for_print = va_arg(more_args, int);
					character_count += _string_integer_initializing(int_for_print, 16, "0x", string_print, (character_count + 1));
					character_count += 1;
					idx_count += 1;

					break;
				case '$':									// for double with prefix $
					double_for_print = va_arg(more_args, double);

					long long n = double_for_print * 100;				
					unsigned long long dollar_abs = n;
					// deals with printing a negative number
					if( n < 0 ) {
						string_print[character_count] = '-';
						//fputc('-', stdout);
						character_count ++;
						dollar_abs = -n;
					}
					
					string_print[character_count] = '$';
					//fputc('$', stdout);
					character_count ++;
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
						string_print[character_count] = ('0' + digit);
						//fputc('0' + digit, stdout);	
						character_count ++;
						if(i == 2){
							//fputc('.', stdout);
							string_print[character_count] = '.';
							character_count ++;
						}
						temporary_long = temporary_long % (power_value);
						power_value = power_value / 10;
					}
					
					idx_count += 1;

					break;
				case 'c':									// for character literal
						
					ascii_val = va_arg(more_args, int);
					string_print[character_count] = ascii_val;
					//fputc(ascii_val, stdout);
					character_count ++;
					idx_count += 1;

					break;
				case 's':									// for string
					string = va_arg(more_args, char *);
					int string_idx_count = 0;
					while (string[string_idx_count] != '\0') {
						string_print[character_count] = string[string_idx_count];
						//fputc(string[string_idx_count], stdout);
						character_count ++;
						string_idx_count +=1;
					}
					idx_count += 1;

					break;
				case '%':									// for putting %
					string_print[character_count] = format[idx_count];
					//fputc('%', stdout);
					character_count ++;
					idx_count += 1;

					break;
				case '\0':									// testing for if % at end
					string_print[character_count] = format[idx_count];
					//fputc('%', stdout);
					character_count ++;
					idx_count += 1;

					break;
				default:									// for everything else
					string_print[character_count] = format[idx_count];
					//fputc('%', stdout);
					character_count ++;
					string_print[character_count] = format[idx_count + 1];
					character_count ++;
					idx_count += 1;

					break;
						

			}
		}

		else{
			string_print[character_count] = format[idx_count];
			//fputc(format[idx_count], stdout);
			character_count ++;
		}
		idx_count += 1;
	}


}

//this is from HW04 modified to count characters instead of print
int _string_integer_initializing(int n, int radix, char* prefix, char* string_print, int index) {

	int character_count = -1;

	unsigned int n_abs = n;
	// deals with printing a negative number
	if( n < 0 ) {
		string_print[(index + character_count)] = '-';
		//fputc('-', stdout);
		character_count ++;
		n_abs = -n;
	}

	// deals with prefix
	int idx_count = 0;
	while(prefix[idx_count] != '\0') {
		string_print[(index + character_count)] = prefix[idx_count];
		//fputc(prefix[idx_count], stdout);
		character_count ++;
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
			string_print[(index + character_count)] = ('W' + digit);
			//fputc('W' + digit, stdout);		
			character_count ++;
		}
		else {
			string_print[(index + character_count)] = ('0' + digit);
			//fputc('0' + digit, stdout);		
			character_count ++;
		}
		temporary_integer = temporary_integer % (power_value);
		power_value = power_value / radix;
	}

	return character_count;
}



//this is from HW04 modified to count characters instead of print
int _count_character_print_integer(int n, int radix, char* prefix) {
	
	int character_count = 0;

	unsigned int n_abs = n;
	// deals with printing a negative number
	if( n < 0 ) {
		character_count ++;
		//fputc('-', stdout);
		n_abs = -n;
	}

	// deals with prefix
	int idx_count = 0;
	while(prefix[idx_count] != '\0') {
		character_count ++;
		//fputc(prefix[idx_count], stdout);
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
			character_count ++;
			//digit += 'a' - ':';
			//fputc('W' + digit, stdout);		
		}
		else {
			character_count ++;
			//fputc('0' + digit, stdout);		
		}
		temporary_integer = temporary_integer % (power_value);
		power_value = power_value / radix;
	}

	return character_count;
}

