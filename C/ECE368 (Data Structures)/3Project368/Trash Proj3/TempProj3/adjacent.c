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
	int distance;
	int total_distance;
	int visited;
	struct Node* prev;
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
		node -> connected = malloc(sizeof(int)*(header->num));//used to be 100000
		node -> distance = -1;
		node -> visited = 0;
		
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
/*
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
*/
void _free_everything(Head* header){
	Node* head = header-> head;
	Node* temp = header ->head;
	while(head != NULL){
		temp = head;
		head = head->next;
		//if(temp -> connected){
		free(temp->connected);
		//}
		free(temp);
	}
	free(header);
}
void _reset_visited(Head* header){
	Node* head = header -> head;
	Node* temp = header -> head;
	while(head != NULL){
		temp = head;
		head = head->next;
		temp -> visited = 0;
		temp -> distance = -1;
	}
}

int _get_distance(Node* start, Node* end){
	//double x_dist = (double)(start->x - end-> x)**2;
	//double y_dist = (double)(start->y - start->y)**2;
	int distance = pow((pow((start->x - end -> x),2)+pow((start->y - end->y),2)),0.5);
	//double adder = pow(
	//double distance = (x_dist + y_dist) ** 0.5;
	return distance;
}

void _get_dist_table(Head* header, Node* head,Node* destination){//
	//if(counter == (finish_index-1)){
	//	return;
	//}
	//Node* temp = header -> head;
	//while(temp-> name != source){
	//	temp = temp-> next;
	//}
	//Node* temp = head;
	//while(temp -> name != source){
	//	temp = temp -> next;
	//}
	
	Node* temp = head;
	if(temp-> visited == 1){
		return;
	}
	temp-> visited = 1;
	int i;
	int temp_name;
	int distance;
	//int total_distance;
	Node* next_node = header->head;
	for(i=0; i < (temp->index_connected); i++){
		temp_name = temp->connected[i];
		while(next_node -> name != temp_name){
			next_node = next_node -> next;
		}
		distance = _get_distance(temp, next_node);
		if ((next_node -> distance == -1)){// || (next_node -> distance > distance)){
			next_node -> prev = temp;
			next_node -> distance = (next_node -> prev -> distance) + distance +1;
		}
		
		//next_node->total_distance = next_node-> total_distance;	
		if (next_node -> distance > distance){
			//next_node->total_distance = -1;
			next_node -> prev = temp;
			next_node -> distance = (next_node -> prev -> distance) + distance;
		}
		
	//	_get_dist_table(header, next_node,destination);
	}
	for(i=0; i < (temp->index_connected); i++){
		temp_name = temp->connected[i];
		while(next_node -> name != temp_name){
			next_node = next_node -> next;
		}
	_get_dist_table(header, next_node,destination);
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
	fclose(fp);
	fp = fopen(argv[2], "r");
	//_print_connected(header);
	
	int num_of_queries = _get_next_num_from_file(fp);
	int source;
	int destination;
	//int* 
	//Node* temp;
	//int finish_index;
	Node* temp1;
	Node* temp2;// = header-> head;
	for(i = 0; i<num_of_queries; i++){
		temp1 = header->head;
		temp2 = header->head;
		source = _get_next_num_from_file(fp); // first is 6
		while(temp1 -> name != source){
			temp1 = temp1 -> next;
		}
		temp1 -> distance = 0;
		destination = _get_next_num_from_file(fp); // end is 23
		while(temp2 -> name != destination){
			temp2 = temp2-> next;
			//finish_index = temp -> index_connected;
		}
		_get_dist_table(header, temp1,temp2);//, finish_index);
		//printf("%f",temp2->prev->distance);
		//printf("start: %d, end: %d\n", source, destination);
	}
	///*
	Node* temp3 = header->head;
	while(temp3 != NULL){
		printf("name: %d ", temp3 -> name);
		printf("x: %d ", temp3 -> x);
		printf("y: %d ", temp3 -> y);
		printf("distance: %d ", temp3 -> distance);
		//printf(" %d", temp3 -> visited);
		printf("visited %d\n", temp3 -> visited);
		temp3 = temp3->next;

	}
	//*/
	fclose(fp);
	_free_everything(header);
	
	
	return EXIT_SUCCESS;
}

