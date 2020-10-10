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

unsigned long long * _data_count_array(FILE * fp, unsigned long long * count_array){
//void _data_count_array(FILE * fp){
	fseek(fp, 0, SEEK_SET);
	//unsigned long long count_array [257] = {0};
	//unsigned long long count_array[257] = malloc(sizeof(unsigned long long) * 257);
	//int data = (int) fgetc(fp);
	char data;

	while((data = fgetc(fp)) != EOF){
		count_array[(int) data]++;
		//data = (int) fgetc(fp);
	}
	//count_array[257] = 1; //fake EOF 														//WAS 256
	return count_array;
}
LinkedList * _data_linked_list(unsigned long long * count_array){
	LinkedList * linked_list = malloc(sizeof(LinkedList));
	int size = 0;
	linked_list -> size = size;
	int i;
	for(i = 0; i < 258; i++){																//WAS 257
		if (count_array[i] != 0){
			//printf("first non-zero: %d\n", i);
			ListNode * node = malloc(sizeof(ListNode));
			node->ASCII = i;
			node->freq = count_array[i];
			node->next = NULL;
			node->is_EOF = 0;

			_add_to_list(linked_list, node);
			linked_list -> size++;

		}
	}
	ListNode* EOF_node = malloc(sizeof(ListNode));
	//EOF_node -> ASCII = 256;
	EOF_node -> freq = 1;
	EOF_node -> next = NULL;
	EOF_node -> is_EOF = 1;

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
		node -> is_EOF = temp -> is_EOF;
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
			///*
			printf("\nleft is  %d before assign\n", left -> freq);
			if(left-> ASCII) printf("left is  %d\n", (int)(left -> ASCII));
			if(left-> is_EOF) printf("left is  %d\n", (int)(left -> is_EOF));
			printf("right is %d before assign\n\n", right -> freq); 
			if(right-> ASCII) printf("right is %d\n", (int)(right -> ASCII));
			if(right-> is_EOF) printf("right is %d\n", (int)(right -> is_EOF));
			//*/
		}
		center -> left = left;
		center -> right = right;
		center -> freq = (left->freq) + (right->freq);
		///*if(left-> ASCII)*/ printf("left is  %c freq %d\n",(char)(left->ASCII),left-> freq);
		//printf("center freq is %d\n", center->freq);
		///*if(right-> ASCII)*/printf("right is %c freq %d\n\n",(char)(right->ASCII),right->freq);
		//center -> ASCII = NULL;
			
		//center -> is_EOF = 0;

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

int _get_height(HeapNode* node){
	if (node == NULL){
		return 0;
	}
	int left = _get_height(node -> left);
	int right = _get_height(node -> right);
	if (left < right){
		return(right+1);
	}
	else{
		return(left+1);
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
		node -> is_EOF = root -> is_EOF;
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
	while(temp -> ASCII != ch || temp -> is_EOF != 1){
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
void _convert_to_huff(FILE * in_fp, FILE * out_fp, LinkedList* table_head, unsigned long long * count_array){
	fwrite(count_array, sizeof(unsigned long long), 257, out_fp);
	char ch;
	unsigned char buffer = 0;
	int count = 0;
	unsigned char bit;
	int i;
	fseek(in_fp, 0, SEEK_SET);
	while((ch = fgetc(in_fp)) != EOF){
		ListNode* node_for_ch = _get_code(ch, table_head);
		for(i=0;i < node_for_ch -> code_index; i++){
			bit = node_for_ch -> code[i];
			buffer <<= 1;
			if (bit){
				buffer |= 1;
			}
			count++;
			if (count == 8){
				fwrite(&buffer, sizeof(buffer), 1, out_fp);
				buffer = 0;
				count = 0;
			}
		}
	}
	//ch = (char) 255;
	//ListNode* node_for_EOF = _get_code(ch, table_head);
	ListNode* temp = table_head -> head;
	while(temp -> is_EOF != 1){
		temp = temp -> next;
	}
	for(i=0;i < temp -> code_index; i++){
		bit = temp -> code[i];
		buffer <<= 1;
		if (bit){
			buffer |= 1;
		}
		count++;
		if (count == 8){
			fwrite(&buffer, sizeof(buffer), 1, out_fp);
			buffer = 0;
			count = 0;
		}
	}
	if (count < 8 && count != 0){
		for(i=0; i+count<8;i++){
			buffer <<= 1;
		}
		fwrite(&buffer, sizeof(buffer), 1, out_fp);
	}
}
///*
void _free_list(ListNode* head){
	ListNode* temp = head;
	while (head != NULL){
		temp = head;
		head = head->next;
		free(temp);
	}
}

void _free_tree(HeapNode* root){
	if (root == NULL){
		return;
	}
	_free_tree(root->left);
	_free_tree(root->right);
	free(root);
}
//*/
int main(int argc, char **argv){
	FILE* fp_in = fopen(argv[1], "r");
	unsigned long long* count_array = malloc(sizeof(unsigned long long) * 257);//I WANT THIS TO BE 257
	count_array = _data_count_array(fp_in, count_array);
	LinkedList* tester = _data_linked_list(count_array);
	Heap* test_heap = _create_heap(tester);
	HeapNode* tree_top = _create_heap_tree(test_heap);
	int code_array[100];
	LinkedList* table_head = malloc(sizeof(LinkedList));
	_get_Codes(tree_top,code_array,0,table_head);
	///*
	ListNode* temp = table_head -> head;
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
	//*/	
	char* filename = malloc(sizeof(char) * (strlen(argv[1])+5));
	strcpy(filename,argv[1]);
	strcpy(&(filename[strlen(argv[1])]), ".huff");
	//printf("%s\n", filename);
	FILE* huff_file = fopen(filename, "w");
	_convert_to_huff(fp_in,huff_file,table_head,count_array);
	
	///*
	//free(count_array);//data count
	_free_list(tester->head);//data count list
	free(tester);
	_free_list(table_head -> head); //table list
	free(table_head);
	_free_tree(tree_top);	//heap 
	free(test_heap -> array);//heap 
	free(test_heap);//heap
	//fclose(fp_in);
	free(filename);
	//fclose(huff_file);
	//*/
////////////////////////////////////////////////////// look at line 137	
	fclose(fp_in);
	fclose(huff_file);
	//printf("%s\n", filename);
	return EXIT_SUCCESS;
}


