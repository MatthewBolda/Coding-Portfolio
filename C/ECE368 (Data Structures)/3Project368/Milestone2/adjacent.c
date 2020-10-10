#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Node{
	int name;
	int x;
	int y;
	int* connected;
	int index_connected;
	struct Node* next;
}Node;

typedef struct Head{
	Node* head;
	int num;
	int cons;
}Head;

void _add_to_list(Head* header, Node* node){
	Node* head = header-> head;
	int inserted = 0;
	if(head == NULL){
		header -> head = node;
	}
	else{
		Node* temp = head;
		while(temp != NULL && inserted == 0){
			if (temp -> next == NULL){
				temp -> next = node;
				inserted = 1;
			}
			else{
				temp = temp -> next;
			}
		}
	}
}

int _get_next_num_from_file(FILE* fp){
	char c;
	c = fgetc(fp);
	//bool not_done = false;
	if(c == 32){
	//	not_done = true;
		while(c == 32){
			c = fgetc(fp);
		}
	}
	int num = 0;
	int temp;
	while((c != 32) && (c  != '\n')){
		temp = atoi(&c);
		num = (num * 10) + temp;
		c = fgetc(fp);
	}
	//printf("%d\n", num);
	return num;
}

void _get_list_nodes(FILE* fp, Head* header){
	int index = 0;
	//char c;
	int name;
	int x;
	int y;
	
	while (index < (header -> num)){
		name = _get_next_num_from_file(fp);
		x = _get_next_num_from_file(fp);
		y = _get_next_num_from_file(fp);
				
		Node* node = malloc(sizeof(Node));
		node -> x = x;
		node -> y = y;
		node -> name = name;
		node -> index_connected = 0;
		node -> connected = malloc(sizeof(int)*100000);
		
		_add_to_list(header,node);
		
		index++;

	}
}

void _fill_connected(FILE* fp, Head* header){
	int starter = _get_next_num_from_file(fp);
	int connected = _get_next_num_from_file(fp);
	Node* temp1 = header-> head;
	Node* temp2 = header-> head;
	while(temp1 -> name != starter || temp2 -> name != connected){
		if(temp1 -> name != starter){
			temp1 = temp1 -> next;
		}
		if(temp2 -> name != connected){
			temp2 = temp2 -> next;
		}
	}
	temp1-> connected[temp1->index_connected] = connected;
	temp1-> index_connected++;
	temp2-> connected[temp2->index_connected] = starter;
	temp2-> index_connected++;

}

void _print_connected(Head* header){
	Node* temp = header-> head;
	int i;
	while(temp != NULL){
		printf("%d:", temp-> name);
		for(i=0; i< temp->index_connected;i++){
			printf(" %d", temp->connected[i]);
		}
		temp = temp -> next;
		printf("\n");
	}
}

int main(int argc, char ** argv){
	FILE* fp = fopen(argv[1], "r");
	int num = _get_next_num_from_file(fp);
	int cons = _get_next_num_from_file(fp);
	//printf("%d", num);
	
	Head* header = malloc(sizeof(Head));
	header-> num = num;
	header-> cons=cons;
	//printf("num: %d, cons: %d\n",num, cons);
	_get_list_nodes(fp, header);
	Node* temp = header->head;
	while(temp != NULL){
		//printf("name: %d, x: %d, y: %d\n", temp->name,temp->x,temp->y);
		temp = temp -> next;
	}

	int i;
	for(i = 0; i<(header->cons)-1; i++){
		_fill_connected(fp, header);
	}

	_print_connected(header);

	
	
	return EXIT_SUCCESS;
}

