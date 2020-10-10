
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"

//helper function declaration
// *a_pos is address of the first character of the integer literal in the input
// *a_value is the location where the parsed int should be stored
// return TRUE if int was found at a_pos, false else
bool parse_int(int* a_value, char** a_pos){
	*a_value = 0;
	
	char* index = *a_pos;
	bool if_negative = false;

	if( *index == '-'){
		index = index + 1;
		if_negative = true;
		if(!(isdigit(*index))){
			return false;
		}
	}

	if( *index >= '0' && *index <= '9'){
		int integer_initializing = 0;
		while( *index >= '0' && *index <= '9'){
			integer_initializing = integer_initializing *10 + (*index - '0');
			index ++;
		}
		*a_value = integer_initializing;
		if( if_negative){
			*a_value = -1 * (*a_value);
		}
		
		*a_pos = index;
		return true;
	}
	else {
		*a_pos = index;
		return false;
	}

}


bool parse_string(char** a_s, char** a_pos){
	*a_s = NULL;
	int length = strlen(*a_pos);
	char* result = malloc((length+1) * sizeof(*result));
	int counter = 0;
	char* index = *a_pos;

	result[counter++] = *index++;
	while( *index != '\"'){
		if( *index == '\n' || *index == '\0'){
			*a_s = result;
			//*a_pos = index;
			result[counter++] = '\0';
			return false;
		}
		result[counter++] = *index++;
	}
	result[counter++] = *index++;
	*a_s = result;
	result[counter++] = '\0';
	*a_pos = index;
	return true;
}

bool parse_list(Node** a_head, char** a_pos){
	//char* index = *a_pos;
	if( **a_pos != '['){
		return false;
	}
	else{
		*a_pos = *a_pos + 1;
	}
	Node* temp_head = *a_head;
	
	Node* new_node;
	//while(**a_pos != ']'){	
		new_node = malloc(sizeof(*new_node));//malloc here?
		if( *a_head == NULL){
			*a_head = new_node;
		}
		while(isspace(**a_pos)){
			*a_pos = *a_pos + 1;
		}

		//while(**a_pos != ']')	
		if(isdigit(**a_pos)){
			parse_element((&(new_node) -> element), a_pos);
		}
		else if( **a_pos == '\"'){
			parse_element((&(new_node) -> element), a_pos);
		}

		if( **a_pos == ','){
			*a_pos = *a_pos + 1;
		}

		if( **a_pos == ']'){
			return true;
		}

		//*a_pos = *a_pos + 1;
		//*a_head= new_node;
		//*a_head = *a_head -> next;
		temp_head = new_node;
		temp_head = (temp_head) -> next;
		
		//new_node = new_node -> next;
		
	//}
	return true;	
}

bool parse_element(Element* a_element, char** a_pos){

	bool parse_worked = false;
	while(isspace(**a_pos)){
		*a_pos = *a_pos + 1;
	}
	if(isdigit(**a_pos) || **a_pos == '-'){
		Element initialize = {.type = ELEMENT_INT, .value.as_int = 0};
		*a_element = initialize;
		parse_worked =parse_int(&a_element -> value.as_int, a_pos);
		return parse_worked;
	}
	else if( **a_pos == '\"'){
		char* test_string= "";
		Element initialize = {.type = ELEMENT_STRING, .value.as_string = test_string};
		*a_element = initialize;
		parse_worked = parse_string(&a_element -> value.as_string, a_pos);
		return parse_worked;
	}
	
	else if( **a_pos == '['){
		
		Node* head = NULL; //{ .next = NULL,.element = NULL};
		Element initialize = {.type = ELEMENT_LIST, .value.as_list = head};
		*a_element = initialize;
		parse_worked = parse_list(&a_element -> value.as_list,a_pos);
		
		return parse_worked;
		
	}
	
	else{
		Element initialize = {.type = ELEMENT_INT, .value.as_int = 0};
		*a_element = initialize;

		return parse_worked;
	}
}

void print_element(Element element){
	if(element.type == ELEMENT_INT){
		printf("%d", element.value.as_int);
	}
	else if(element.type == ELEMENT_STRING){
		printf("%s", element.value.as_string);
	}
	
	else if(element.type == ELEMENT_LIST){
		printf("[");
		
		for(Node* curr = element.value.as_list; curr != NULL; curr = curr -> next){
			///// print_element(curr -> element); //maybe works idk
			if(curr -> element.type == ELEMENT_INT){
				printf("%d", curr -> element.value.as_int);
			}
			else if(curr -> element.type == ELEMENT_STRING){
				printf("%s", curr -> element.value.as_string);
			}
		}
		printf("]");
		
		//for loop notation, check type, print type, go to next, repeat
		//IDK DO SOMETHING RECURSIVE
	}
	
}

void free_element(Element element){
	if(element.type == ELEMENT_STRING){
		free(element.value.as_string);
	}
	
	if(element.type == ELEMENT_LIST){
		while( element.value.as_list != NULL){
			Node* free_node = element.value.as_list;
			element.value.as_list = free_node -> next;
			if(free_node -> element.value.as_string == NULL){
				free(free_node -> element.value.as_string);
			}
			free(free_node);
		}
	}
	
	//else if(element.type == element_list){
	// //IDK DO SOMETHING RECURSIVE TO FREE LIST
	//}
	//else{
	//	return;
	//}
	
	// this is basically how to delete a list, delete later
	/*
	while( a_list -> head != NULL) {
		ListNode* free_node = a_list -> head;
		a_list -> head = a_list -> head -> next;
		free(free_node);
	}
	*/
}

