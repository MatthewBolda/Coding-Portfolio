#ifndef __JSON_H_20190306_105300__
#define __JSON_H_20190306_105300__
#define JSON_C11  // This symbol may be used in grading.
#include <stdbool.h>

typedef struct {
	enum  { ELEMENT_INT, ELEMENT_STRING, ELEMENT_LIST } type;
	union {
		int    as_int;
		char*  as_string;
		struct _Node* as_list;
	}; // ANONYMOUS union -- Requires update to C11.  See instructions in json_c11.h.
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

// HOW TO UPDATE TO C11
//
// 1. Get the updated .bashrc file.  From bash:
//    cp -i ~ece264/19sp/.bashrc ~
//
// 2. Edit your Makefile.  Replace "-std=c99" with "-std=c11"
//
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
