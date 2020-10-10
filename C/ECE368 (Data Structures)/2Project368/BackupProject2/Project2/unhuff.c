#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huff.h"

void _add_to_list(LinkedList * linked_list, ListNode * node){
	ListNode * head = linked_list -> head;
	int inserted = 0;
	if( head == NULL){
		linked_list -> head = node;
		//node -> next = head;
	}
	else{
		ListNode * temp = head;
		while(temp != NULL && inserted == 0){
			if (temp -> next == NULL){
				temp -> next = node;
				inserted = 1;
			}
			else{
				temp = temp -> next;
			}
		}
		//temp = node;
	}
}

LinkedList * _data_linked_list(unsigned long long * count_array){
	LinkedList * linked_list = malloc(sizeof(LinkedList));
	int size = 0;
	linked_list -> size = size;
	int i;
	for(i = 0; i < 256; i++){
		if (count_array[i] != 0){
			//printf("first non-zero: %d\n", i);
			ListNode * node = malloc(sizeof(ListNode));
			node->ASCII = i;
			node->freq = count_array[i];
			node->next = NULL;

			_add_to_list(linked_list, node);
			linked_list -> size++;

		}
	}
	ListNode* EOF_node = malloc(sizeof(ListNode));
	EOF_node -> ASCII = 256;
	EOF_node-> freq = 1;
	EOF_node-> next = NULL;
	
	_add_to_list(linked_list, EOF_node);
	linked_list -> size++;
	return linked_list;
}


Heap * _create_heap(LinkedList * list){
	Heap* heap = malloc(sizeof(Heap));
	heap -> current_size = 0;
	heap -> original_size = list -> size;
	heap -> array = malloc(sizeof(HeapNode*) * heap -> original_size);
	int i;
	ListNode* temp = list -> head;
	for(i = 0; i < heap -> original_size; i++){
		HeapNode* node = malloc(sizeof(HeapNode*));
		node -> left = NULL;
		node -> right = NULL;
		node -> ASCII = temp -> ASCII;
		node -> freq = temp -> freq;
		temp = temp -> next;
		heap-> array[i] = node;
		heap-> current_size++;
	}
	for(i = (heap->current_size - 2) / 2;i >= 0;i--){
		_heapify(heap,i);
	}
	return heap;	

}

HeapNode* _create_heap_tree(Heap* heap){
	HeapNode* right;
	HeapNode* left;
	//HeapNode* center;
	while(heap -> current_size != 1){
		//get least used element
		left = heap->array[0];
		heap->array[0] = heap->array[(heap-> current_size) - 1];
		//printf("left is  %d before heapify\n", left -> freq); 
		heap->current_size--;
		_heapify(heap,0);
		//get second least used element
		right = heap->array[0];
		heap->array[0] = heap->array[(heap-> current_size) - 1];
		//printf("right is %d after heapify\n", right -> freq); 
		heap->current_size--;
		_heapify(heap,0);
		
		heap->current_size++;
		HeapNode* center = malloc(sizeof(HeapNode*));
		if( left -> freq > right -> freq){
			HeapNode* temp = right;
			right = left;
			left = temp;
			//printf("\nleft is  %d before assign\n", left -> freq);
			//if(left-> ASCII) printf("left is  %d\n", (int)(left -> ASCII));
			//printf("right is %d before assign\n\n", right -> freq); 
			//if(right-> ASCII) printf("right is %d\n", (int)(right -> ASCII));
		}
		center -> left = left;
		center -> right = right;
		center -> freq = (left->freq) + (right->freq);
		///*if(left-> ASCII)*/ printf("left is  %c freq %d\n",(char)(left->ASCII),left-> freq);
		//printf("center freq is %d\n", center->freq);
		///*if(right-> ASCII)*/printf("right is %c freq %d\n\n",(char)(right->ASCII),right->freq);
		//center -> ASCII = NULL;
		heap -> array[heap->current_size - 1] = center;
		_heapify(heap,0);
	}
	return heap->array[0];
}

//HeapNode* _get_least_used(Heap* heap){
	
//}

void _heapify(Heap* heap, int index){
	int parent = index;
	int left_child = (2 * index) + 1;
	int right_child = (2* index) + 2;
	
	if(left_child < heap -> current_size){
		if(heap->array[left_child]-> freq < heap->array[parent] -> freq){
			parent = left_child;
		}
	}
	if(right_child < heap -> current_size){
		if(heap->array[right_child]-> freq < heap->array[parent] -> freq){
			parent = right_child;
		}
	}
	if(parent != index){
		HeapNode* temp = heap->array[parent];
		heap->array[parent] = heap->array[index];
		heap->array[index] = temp;
		_heapify(heap,parent);
	}
}

void _get_Codes(HeapNode* root, int coder[], int index, LinkedList* table_head){
	//LinkedList* table_head = malloc(sizeof(LinkedList));
	if (root -> left != NULL){
		coder[index] = 0;
		_get_Codes(root->left, coder, index + 1,table_head);
	}
	if (root -> right != NULL){
		coder[index] = 1;
		_get_Codes(root->right, coder, index + 1,table_head);
	}
	if ((root -> left == NULL) && (root -> right == NULL)){
		//if (root -> ASCII == '\n'){
		//	printf("       char:\\nfreq: %d code: ",root -> freq);
		//}
		//else if (root -> ASCII == 257 || root ->ASCII == 256){
		//	printf("       char:EOFfreq: %d code: ",root -> freq);
		//}
		//else printf("       char: %c freq: %d code: ", root -> ASCII,root -> freq);
		//printArr(coder,top);
		ListNode * node = malloc(sizeof(ListNode));
		node -> ASCII = root ->ASCII;
		//node -> code = coder;
		node -> freq = root -> freq;
		node -> code_index = index;
		//int temp_code[index];
		//int i = 0;
		//for(i=0;i<index;i++){
		//	temp_code[i] = coder[i];
		//}
		//node -> code = temp_code;
		node -> next = NULL;
		_add_to_list(table_head, node);
		//int* temp_code = malloc(sizeof(int*));
		int i;
		int ch;
		for(i=0;i< index; i++){
			ch = coder[i];
			//printf("%d", coder[i]);
			node -> code[i] = ch;
		}
		//node -> code = temp_code;
		//free(temp_code);
		//printf("\n");
	}
	/*
	ListNode* temp = table_head -> head;
	while(temp != NULL){
		if (root -> ASCII == '\n'){
			printf("table: char:\\nfreq: %d code: ",root -> freq);
		}
		else if (root -> ASCII == 257 || root ->ASCII == 256){
			printf("table: char:EOFfreq: %d code: ",root -> freq);
		}
		else printf("table: char: %c freq: %d code: ", root -> ASCII,root -> freq);
		//printf("\n");
		int i;
		for(i=0;i< temp-> code_index ; i++){
			printf("%d", temp -> code[i]);
		}
		printf("\n");
		temp = temp -> next;
	}*/
	//return table_head;
}

ListNode*  _get_code(char ch, LinkedList* table_head/*, char * code*/){
	ListNode* temp = table_head -> head;
	while(temp -> ASCII != ch){
		temp = temp -> next;
	}
	return temp;
	//char code[100];
	//char character;
	//int i;
	//for(i=0;i < temp -> code_index; i++){
		//character = temp -> code[i];
	//	code[i] = temp -> code[i];
	//}
	//return code;
}

void _convert_to_unhuff(FILE* fp_in,FILE* unhuff_file,HeapNode* tree_top){
	HeapNode* root = tree_top;
	int i;
	char bit;
	int incomplete = 1;
	unsigned char buffer = 0;
	int mask = 1 << 7;
	char ch;
	while(incomplete == 1){
	//int j;
	//for(j=0;j<1;i++){
		buffer = fgetc(fp_in);
		for(i=0; i < 8; i++){
			bit = buffer & mask;
			buffer = buffer << 1;
			//printf("i= %d, bit = %d\n",i, bit);
			if (bit == 0){
				root = root->left;
			}
			else{
				root = root->right;
			}
			if(root -> ASCII){
				ch = root -> ASCII;
				//if (ch == 300 || ch == 44 || ch == 43 || ch == 46){
				if (ch == 0){
					//printf("it hit");
					incomplete = 0;
					break;
				}
				root = tree_top;
				//printf("%c\n", ch);
				fputc(ch,unhuff_file);
			}
		}
		/*
		printf("%d\n", bit);
		buffer = buffer << 1;
		bit = buffer & mask;
		printf("%d", bit);
		//for(i=0; i< 8;i++){
			//char read_bit = 
		//}
		*/
	}

}

int main(int argc, char **argv){
	FILE* fp_in = fopen(argv[1], "r");
	unsigned long long* count_array = malloc(sizeof(unsigned long long) * 257);
	fread(count_array, sizeof(unsigned long long), 257, fp_in);

	//count_array = _data_count_array(fp_in, count_array);
	LinkedList* tester = _data_linked_list(count_array);
	Heap* test_heap = _create_heap(tester);
	HeapNode* tree_top = _create_heap_tree(test_heap);
	int code_array[100];
	LinkedList* table_head = malloc(sizeof(LinkedList));
	_get_Codes(tree_top,code_array,0,table_head);
	/*
	ListNode* temp = table_head -> head;
	printf("%d\n", tree_top -> freq);
	while(temp != NULL){
		if (temp -> ASCII == '\n'){
			printf("table: char:\\n freq: %d size of code: %d code: ", temp -> freq, temp -> code_index);

		}
		else if ( temp ->ASCII == 257){
			printf("table: char:EOFfreq: %d size of code: %d code: ", temp -> freq, temp -> code_index);
		}
		else printf("table: char: %c freq: %d size of code: %d code: ", temp -> ASCII,temp -> freq, temp -> code_index);
		//printf("\n");
		int i;
		for(i=0;i< temp-> code_index ; i++){
			printf("%d", temp -> code[i]);
		}
		printf("\n");
		temp = temp -> next;
	}
	*/
	///*
	char* filename = malloc(sizeof(char) * strlen(argv[1]+7));
	strcpy(filename,argv[1]);
	strcpy(&(filename[strlen(argv[1])]), ".unhuff");
	//printf("%s\n", filename);
	FILE* unhuff_file = fopen(filename, "w");
	_convert_to_unhuff(fp_in,unhuff_file, tree_top);
	fclose(fp_in);
	fclose(unhuff_file);
	//*/
	return EXIT_SUCCESS;
}

