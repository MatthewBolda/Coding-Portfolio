#include <stdbool.h>
#include <stdlib.h>
#ifndef __sorts_h__
#define __sorts_h__

typedef struct List {
	struct ListNode* head;
	struct ListNode* tail;
	int    			 size;
} List;

typedef struct ListNode {
	int    			 value;
	struct ListNode* next;
}ListNode;

typedef struct BST {
	struct BSTNode* root;
	int    			size;
}BST;

typedef struct BSTNode {
	int    			value;
	struct BSTNode* left;
	struct BSTNode* right;
}BSTNode;

void merge_sort_array(int* array, size_t size);
void tree_sort_array(int* array, size_t size);
void quick_sort_array(int* array, size_t size);
List create_list(const int* array, int size);
void merge_sort_list(List* a_list);
void empty_list(List* a_list);
BST create_bst(const int* array, int size);
void empty_bst(BST* bst);


#endif
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
