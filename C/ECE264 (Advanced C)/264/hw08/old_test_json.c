

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"

bool _test_parse_int(char* input_string);
void _bool_print(bool* did_it_work);
bool _test_parse_string(char* input_string);
bool _test_parse_element(char* input_string);


bool _test_parse_int(char* input_string){
	int test_value = 0;
	bool did_it_work = parse_int(&test_value, &input_string);
	if(did_it_work){
		printf("%d\n", test_value);
	}
	return did_it_work;
}
bool _test_parse_string(char* input_string){
	char* test_string = "";
	bool did_it_work = parse_string(&test_string, &input_string);
	if(did_it_work){
		printf("%s\n", test_string);
	//	free(test_string);
	}
	free(test_string);
	return did_it_work;
}

bool _test_parse_element(char* input_string){
	Element element;
	bool did_it_work = parse_element(&element, &input_string);
	if(did_it_work){
		print_element(element);
		free_element(element);
		printf("\n");
	}
	else {
		free_element(element);
	}
	return did_it_work;
}



void _bool_print(bool* did_it_work){
	if(*did_it_work){
		printf("TRUE\n");
	}
	else{
		printf("FALSE\n");
	}
}

int main(int arc, char*argv[]) {
	//testing parse_int

	bool did_it_work = _test_parse_int("1");
	_bool_print(&did_it_work);					// should be true	
	did_it_work = _test_parse_int("2147483647");
	_bool_print(&did_it_work);					// should be true
	did_it_work = _test_parse_int("-2147483648");
	_bool_print(&did_it_work);					// should be true
	did_it_work = _test_parse_int("-A9");
	_bool_print(&did_it_work);					// should be false
	did_it_work = _test_parse_int("-123-456");
	_bool_print(&did_it_work);					// should be true
	did_it_work = _test_parse_int("A9");
	_bool_print(&did_it_work);					// should be false
	
/*
	int test_value = 0;
	char* input_string = "-A9";
	did_it_work = parse_int(&test_value, &input_string);

		printf("\n\n\n\n=================================================\n");	
		printf("%d\n", test_value);
		printf("%s\n", input_string);
		_bool_print(&did_it_work);								// should be false
		printf("=================================================\n\n\n\n");	
*/




	printf("\nDone testing parse_int\n\n");

	//testing parse_string
	
	did_it_work = _test_parse_string("\"ABCDE123abcde\"");
	_bool_print(&did_it_work);								//should be true
	did_it_work = _test_parse_string("\"string\\\"");
	_bool_print(&did_it_work);								// should be true
	did_it_work = _test_parse_string("\"split\nstring\"");
	_bool_print(&did_it_work);								// should be false
	did_it_work = _test_parse_string("\"split \\a string with slash a\"");
	_bool_print(&did_it_work);								// should be true
	did_it_work = _test_parse_string("\"backslash string\\ test\"");
	_bool_print(&did_it_work);								//should be true
	did_it_work = _test_parse_string("\"string with no quote");
	_bool_print(&did_it_work);								// should be false
	did_it_work = _test_parse_string("\"string\"sadbfaba");
	_bool_print(&did_it_work);								// should be true


/*
	char* test_string = "";
	 input_string = "\"this doesnt matter";
	did_it_work = parse_string(&test_string, &input_string);

		printf("\n\n\n\n=================================================\n");	
		printf("%s\n", test_string);
		printf("%s\n", input_string);
		_bool_print(&did_it_work);		// should be false
		printf("=================================================\n\n\n\n");	
		free(test_string);
*/




	printf("\nDone testing parse_string\n\n");

	//testing parse_element
	
	did_it_work = _test_parse_element("12345");
	_bool_print(&did_it_work);								//should be true
	did_it_work = _test_parse_element("  \"string with empty space\"");
	_bool_print(&did_it_work);								//should be true
	did_it_work = _test_parse_element("   12345");
	_bool_print(&did_it_work);								//should be true
	did_it_work = _test_parse_element("-123 123");
	_bool_print(&did_it_work);								//should be true
	did_it_work = _test_parse_element("123\n123");
	_bool_print(&did_it_work);								//should be true
	did_it_work = _test_parse_element("string with no quotes");
	_bool_print(&did_it_work);								//should be false
	did_it_work = _test_parse_element("\"string with \n newline\"");
	_bool_print(&did_it_work);								//should be false
	
	
	printf("\nDone testing parse_element\n\n");
	
	//testing parse_list

	//did_it_work = _test_parse_element("[ ]");
	//_bool_print(&did_it_work);								//should be false
	//did_it_work = _test_parse_element("[1]");
	//_bool_print(&did_it_work);								//should be false
	did_it_work = _test_parse_element("[\"Matt\"]");
	_bool_print(&did_it_work);								//should be false
	//did_it_work = _test_parse_element("[1,2]");
	//_bool_print(&did_it_work);								//should be false
	/*
	char* test_string = "[ ]";
	did_it_work = parse_element(&test_string, &input_string);
	if(did_it_work){
		printf("Yes it worked \n %s\n", test_string);
	}
	*/
	return EXIT_SUCCESS;
}
