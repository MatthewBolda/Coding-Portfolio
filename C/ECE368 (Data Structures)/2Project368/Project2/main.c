#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huff.h"

int main(int argc, char **argv){
	//
	//FILE * fp = fopen("testfile.txt", "r");
	FILE * fp = fopen("tester.txt", "r");
	unsigned long long *count_array = malloc(sizeof(unsigned long long) * 257);
	count_array = _data_count_array(fp, count_array);
	//fclose(fp);
	LinkedList* tester = _data_linked_list(count_array);
	//printf("tester: data = %d, count = %d", (char) tester-> head -> ASCII, tester -> head -> freq);
	//ListNode* temp = tester -> head;
	//while(temp != NULL){
		//printf("tester: data = %d, count = %d", (char) temp -> ASCII, temp -> freq);
		//printf("\n");
	//	temp = temp -> next;
	//}
	//printf("size = %d \n", tester -> size);
	Heap* test_heap = _create_heap(tester);
	HeapNode* tree_top = _create_heap_tree(test_heap);
	//int tree_height = _get_height(tree_top);
	int code_array[100];								/// change to tree_height or something
	//int indexer = 0;
	//char ** table = malloc(sizeof(char) * 100);
	//char * result_string = malloc(sizeof(char) * 1024);

	//_fill_codes(tree_top,
	LinkedList* table_head = malloc(sizeof(LinkedList));
	_get_Codes(tree_top, code_array, 0, table_head);
	//_heapify(test_heap,1);
	ListNode* temp = table_head -> head;
	while(temp != NULL){
		if (temp -> ASCII == '\n'){
			printf("table: char:\\n freq: %d size of code: %d code: ", temp -> freq, temp -> code_index);

		}
		else if (/*temp -> ASCII == 257 ||*/ temp ->ASCII == 256){
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

	//char* filename = malloc(sizeof(char) * (strlen(argv[1])+5));
	//strcpy(filename,argv[1]);
	//strcpy(&(filename[strlen(argv[1])]), ".huff");
	char* filename = malloc(sizeof(char) * (strlen("tester.txt")+6));
	strcpy(filename,"tester.txt");
	strcpy(&(filename[strlen("tester.txt")]), ".hufff");
	printf("%s\n", filename);
	
	//ListNode* code_for_g =_get_code('g', table_head);
	//ListNode* code_for_o =_get_code('o', table_head);
	//printf("\n%d%d\n\n", code_for_g -> code[0], code_for_g -> code[1]);
	//printf("\n%d%d\n\n", code_for_o -> code[0], code_for_o -> code[1]);
	FILE* huff_file = fopen(filename, "w");
	_convert_to_huff(fp, huff_file, table_head, count_array); 
	if (tree_top){
		printf("please work");
	}
	fclose(fp);
	fclose(huff_file);
	return 0;
	//TESTER
}
