/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"

bool parse_int(int* a_value, char** a_pos){
	//initialize values
	*a_value = 0;
	bool if_negative = false;
	
	//check of negative integer
	if( **a_pos == '-'){
		*a_pos = *a_pos + 1;
		if_negative = true;
		if(!(isdigit(**a_pos))){
			return false;
		}
	}

	//check if actually integer, initialize integer
	if( **a_pos >= '0' && **a_pos <= '9'){
		int integer_initializing = 0;
		while( **a_pos >= '0' && **a_pos <= '9'){
			integer_initializing = integer_initializing *10 + (**a_pos - '0');
			*a_pos = *a_pos + 1;
		}
		*a_value = integer_initializing;
		if( if_negative){
			*a_value = -1 * (*a_value);
		}
		return true;
	}

	return false;

}


bool parse_string(char** a_s, char** a_pos){
	// initialize string,
	*a_s = NULL;
	int length = strlen(*a_pos);
	char* index = *a_pos;
	if( length == 0 || *index != '\"' ){
		return false;
	}
	char* result = malloc((length-1) * sizeof(*result));
	int counter = 0;

	//run through string, checking if it has invalid characters, initializing return string
	index++;
	while( *index != '\"'){
		if( *index == '\n' || *index == '\0'){
			free(result);
			return false;
		}
		else{
			result[counter++] = *index++;
		}
	}
	
	//update a_pos to character after string, add null terminator to return string
	index++;
	*a_s = result;
	result[counter++] = '\0';
	*a_pos = index;
	return true;
}

bool parse_element(Element* a_element, char** a_pos){
	
	//initialize boolean and skip whitespace
	bool parse_worked = false;
	while(isspace(**a_pos)){
		*a_pos = *a_pos + 1;
	}
	
	//check if integer for parse_int
	if(isdigit(**a_pos) || **a_pos == '-'){
		Element initialize = {.type = ELEMENT_INT, .value.as_int = 0};
		*a_element = initialize;
		parse_worked =parse_int(&a_element -> value.as_int, a_pos);
	}
	
	//check if string for parse_string
	else if( **a_pos == '\"'){
		char* test_string= "";
		Element initialize = {.type = ELEMENT_STRING, .value.as_string = test_string};
		*a_element = initialize;
		parse_worked = parse_string(&a_element -> value.as_string, a_pos);
	}

	//check if list for parse_list
	else if( **a_pos == '['){
		Element initialize = {.type = ELEMENT_LIST, .value.as_list = test_string};
		*a_element = initialize;
		parse_worked = parse_string(
	return parse_worked;
}

void print_element(Element element){
	if(element.type == ELEMENT_INT){
		printf("%d", element.value.as_int);
	}
	else if(element.type == ELEMENT_STRING){
		printf("%s", element.value.as_string);
	}
	else{
		for(Node* curr = element.value.as_list; curr != NULL; curr = curr -> next){
			printf("[");
			print_element(curr -> element);
			printf("]");
		}
	}
}

void free_element(Element element){
	if(element.type == ELEMENT_STRING){
		free(element.value.as_string);
	}
	if(element.type == ELEMENT_LIST){
		while(element.value.as_list != NULL){
			Node* temp = element.value.as_list;
			element.value.as_list = element.value.as_list -> next;
			free(temp);
		}
	}
}

