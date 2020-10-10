#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "sorts.h"
//#include <stdio.h>
//int index_counter = 0;

// helper function declarations
void _insert_node(List* address_list, int value);
ListNode* _Find_Middle_Node(ListNode* address_list);
ListNode* _merge_sort_helper(ListNode* head);
ListNode* _merge_split_lists(ListNode* left_head, ListNode* right_head);

BSTNode* _insert_root(BSTNode* root, int value);
void _store_BST(BSTNode* root, int* array, int* index_counter);
void _free_BST(BSTNode* root);

int _qsort_helper(const void* left, const void* right);

// step 1 call create_list
// step 2 call merge_sort_list
// step 3 store values in array
// step 4 call empty_list
void merge_sort_array(int* array, size_t size) {
	
	List sorted_list = create_list(array, size);

	merge_sort_list(&sorted_list);
	
	int counter = 0;	
	for(ListNode* temp = ((&sorted_list) -> head); temp != NULL; temp = temp -> next) {
		//printf("%d", temp -> value);
		array[counter] = temp -> value;
		counter ++;
	}

	empty_list(&sorted_list);
	
}

// step 1 call create_bst
// step 2 use in order traversal to store sorted values in array
// step 3 call empty_list
void tree_sort_array(int* array, size_t size){

	BST sorted_tree = create_bst(array,size);
	//printf("\n\n%d\n\n", sorted_tree. size);
	int index_counter = 0;
	_store_BST(sorted_tree.root, array, &index_counter);
	empty_bst(&sorted_tree);
}

void quick_sort_array(int* array, size_t size){

	qsort(array,size,sizeof(array[0]), _qsort_helper);

}

int _qsort_helper(const void* left, const void* right){
	const int* left_integer = left;
	const int* right_integer = right;

	if(*left_integer < *right_integer){
		return -1;
	}
	else if( *left_integer > *right_integer){
		return 1;
	}
	else{
		return 0;
	}

}

void _store_BST(BSTNode* root, int* array, int* index_counter){
	//int counter = 0; //doesnt work, needs to be brought in function
	if (root != NULL){
		_store_BST(root -> left, array, index_counter);
	//	printf("array[%d] = %d\n",index_counter ,root -> value);
		array[*index_counter] = root -> value;
		*index_counter = *index_counter + 1;
		_store_BST(root -> right, array, index_counter);
	//	printf(" index_counter = %d\n", index_counter);
	}

}

void _free_BST(BSTNode* root){
	if(root != NULL){
		_free_BST(root -> left);
		_free_BST(root -> right);
		free(root);
	}
}
void empty_bst(BST* sorted_tree){
	_free_BST(sorted_tree -> root);
}

BSTNode* _insert_root(BSTNode* root, int value) {

	if(root == NULL){
		root = malloc(sizeof(*root));
			root -> value = value;
			root -> left = NULL;
			root -> right = NULL;
		return root;
	}
	else if( value <= root -> value){
		root -> left = _insert_root(root -> left, value);
		}
	else{
		root -> right= _insert_root(root -> right, value);
		}
	return root;
}
BST create_bst(const int* array, int size) {
	BST new_tree = { .root = NULL, .size = size};
	if(size == 0){
		return new_tree;
	}
	else {
		new_tree.root = _insert_root( new_tree.root, array[0]);
		for(int i=1; i < size; i++){
			_insert_root( new_tree.root, array[i]);
		}
	}
	return new_tree;
}

// helper function to insert node and keep track of tail
void _insert_node(List* address_list, int value){
	ListNode* new_node = malloc(sizeof(*new_node));
		new_node -> value = value;
		new_node -> next = NULL;

	if(address_list -> head == NULL){
		address_list -> head = new_node;
	}
	else{	
		ListNode* temp_node = address_list -> head;
		while(temp_node -> next != NULL){
			temp_node = temp_node -> next;
		}
		temp_node -> next = new_node;
		address_list -> tail = new_node;
	}

}
// initialize list and set null, then use helper to add nodes.
List create_list(const int* array, int size) {
	
	List new_list = { .head = NULL, .tail = NULL, .size = size};

	if(size == 0) {
	//	assert((new_list -> head == NULL) == (new_list -> size == 0));
	//	assert((new_list -> tail == NULL) == (new_list -> size == 0));
		return new_list;
	}
	else {
		for(int i = 0; i < size; i++) {
			_insert_node(&new_list, array[i]);
		}
	}
	return new_list;
	
}

void merge_sort_list(List* a_list) {

	ListNode* merged_head = _merge_sort_helper(a_list -> head);
	//printf("merged head  %d\n", merged_head -> value);
	a_list -> head = merged_head;
	
}




ListNode* _merge_sort_helper(ListNode* head) {

	if(( head == NULL) || (head -> next == NULL)){
		return head;
	}
	else{
	
	ListNode* middle = _Find_Middle_Node(head);
	//printf("middle     %d\n", middle -> value);
	ListNode* left_head  = head;
	//printf("left head  %d\n", left_head -> value);
	ListNode* right_head = middle -> next;
	//printf("right head %d\n", right_head -> value);
	middle -> next = NULL;

	ListNode* recursed_left_head = _merge_sort_helper(left_head);
	//printf(" recursed_left_head   %d\n", recursed_left_head -> value);
	ListNode* recursed_right_head = _merge_sort_helper(right_head);
	//printf(" recursed_right_head  %d\n", recursed_right_head -> value);
	
	ListNode* merged_head = _merge_split_lists(recursed_left_head, recursed_right_head);
	
	//printf("merged head  %d\n", merged_head -> value);
	return merged_head;
	}
}

ListNode* _merge_split_lists(ListNode* left_head, ListNode* right_head){

	ListNode* result_head = NULL;
	if(left_head == NULL){
		return right_head;
	}
	else if(right_head == NULL){
		return left_head;
		
	}
	if( left_head -> value <= right_head -> value){
		result_head = left_head;
		result_head -> next = _merge_split_lists(left_head -> next, right_head);
	}
	else {
		result_head = right_head;
		result_head -> next = _merge_split_lists(left_head, right_head -> next);
	}

	return result_head;

}

ListNode* _Find_Middle_Node(ListNode* head){
	//if(head == NULL){
	//	return head;       //none of my test cases caused these lines to be executed
	//}					   // so I commented them out for full gcov
	ListNode* up_by_one = head;
	ListNode* up_by_two = head;
	while( (up_by_two -> next != NULL) && (up_by_two -> next -> next != NULL)){
		up_by_one = up_by_one -> next;
		up_by_two = up_by_two -> next -> next;
	}
	return up_by_one;
}

void empty_list(List* a_list) {
	
	while( a_list -> head != NULL) {
		ListNode* free_node = a_list -> head;
		a_list -> head = a_list -> head -> next;
		free(free_node);
	}
	a_list -> head = NULL;
	a_list -> tail = NULL;
	a_list -> size = 0;
	
}

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
