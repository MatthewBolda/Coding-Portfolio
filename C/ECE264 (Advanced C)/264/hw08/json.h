#ifndef __JSON_H_20190302_125600__
#define __JSON_H_20190302_125600__
#include <stdbool.h>

typedef struct {
	enum  { ELEMENT_INT, ELEMENT_STRING, ELEMENT_LIST } type;
	union { int    as_int;
		    char*  as_string;
		    struct _Node* as_list; } value;
} Element;

typedef struct _Node {
	struct _Node* next;
	Element element;
} Node;

bool parse_int(int* a_value, char** a_pos);
bool parse_string(char** a_s, char** a_pos);
bool parse_element(Element* a_element, char** a_pos);
bool parse_list(Node** a_head, char** a_pos);
void print_element(Element element);
void free_element(Element element);
#endif
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
