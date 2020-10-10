#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "new.h"

int _get_next_num_from_file(FILE* fp){
	int num;
	fscanf(fp, "%d", &num);
	return num;
}

int _get_distance(Node* start, Node* end){
	int distance = pow((pow((start->x - end -> x),2)+pow((start->y - end->y),2)),0.5);
	return distance;
}

void _get_list_nodes(FILE* fp, Head* header, Node** harray){
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
		node -> distance = 100000;
		node -> visited = 0;
		//TODO 1
		if ( name > 87600){
			printf("test");
		}
		
		_add_to_list(header,node,harray);
		harray[index] = node;
		index++;

	}
}
void _add_to_list(Head* header, Node* node, Node** harray){
	Node* head = header-> head;
	//int inserted = 0;
	int temp_name;
	Node* temp_node;
	if(head == NULL){
		header -> head = node;
	}
	else{
		temp_name = node->name;
		temp_node = harray[temp_name-1];
		temp_node -> next = node;
	}
}
void _fill_connected(FILE* fp, Head* header, Node** harray){
	int starter = _get_next_num_from_file(fp);
	int connected = _get_next_num_from_file(fp);
	Node* temp1 =harray[starter];// header-> head;//harray[starter]
	Node* temp2 =harray[connected];// header-> head;//harray[connected];
	temp1-> connected[temp1->index_connected] = connected;
	temp1-> index_connected++;
	temp2-> connected[temp2->index_connected] = starter;
	temp2-> index_connected++;

}

void _min_heapify(Node** harray, int* position, int index, int queue_num){
	/*
	int smallest = index;
	int left = (index*2) + 1;
	int right = (index*2) + 2;
	Node* temp1;
	Node* temp2;
	Node* temp3;

	if(left < queue_num && harray[left]->distance < harray[smallest]->distance){
		smallest = left;
	}
	if(right < queue_num && harray[right]->distance < harray[smallest]->distance){
		smallest = right;
	}
	if(smallest != index){
		temp1 = harray[smallest];
		temp2 = harray[index];

		position[temp1->name] = index;
		position[temp2->name] = smallest;

		temp3 = temp1;
		temp1 = temp2;
		temp2 = temp3;
		_min_heapify(harray, position,smallest, queue_num);

	}
	*/
	///*
	Node* checker = harray[index -1];
	int parent_name = harray[index-1]->name;
	int child;

	while(index < (queue_num / 2)){
		child = index*2;//right child, -1 is left child
		if (child < queue_num){
			if(harray[child - 1] ->distance == -1 || harray[child]->distance == -1){
				child = child + 1;
			}
			else if(harray[child - 1]->distance >= harray[child]->distance){// || harray[child - 1]-> distance == -1){ //TODO fix??
				child = child + 1;
			}
		}
		if(checker -> distance >= harray[child - 1] -> distance){
			harray[index - 1] = harray[child - 1];
			position[harray[child -1]->name] = index -1;
			index = child;
			harray[index-1] = checker;
			//harray[child] = index - 1;
			position[parent_name] = index - 1; 
		}
		else{
			index = queue_num / 2;
		}

	}
	//*/
}

Node* _pull_min(Node** array, int* position, int size){

	Node* pulled = array[0];
	
	array[0] = array[size-1];
	array[size-1] = pulled;
	
	position[array[size-1]->name] = 0;
	position[pulled->name] = size-1;

	_min_heapify(array, position, 1, size - 2);
	return pulled;
}


int main(int argc, char ** argv){
	FILE* fp = fopen(argv[1], "r");
	int num = _get_next_num_from_file(fp);
	int cons = _get_next_num_from_file(fp);

	int* position = malloc(num * sizeof(int));
	Head* header = malloc(sizeof(Head));
	header-> num = num;
	header-> cons = cons;
	Node* harray[num];
	_get_list_nodes(fp,header,harray);
	int i;
	for(i = 0; i<(header->cons); i++){
		_fill_connected(fp, header,harray);
	}
	fclose(fp);

	fp = fopen(argv[2], "r");
	int num_of_queries = _get_next_num_from_file(fp);
	if(num_of_queries){}

	int source;
	int destination;
	int queue_num;
	int num_complete;
	int j;
	int k;
	Node* pulled;
	Node* connected;
	int connected_name;
	int distance;
	int total_distance;
	//for(i = 0; i<num_of_queries; i++){
		source = _get_next_num_from_file(fp); 
		destination = _get_next_num_from_file(fp);
		harray[source]->distance = 0;
		//TODO 1

		queue_num = num;
		num_complete = 0;
		for(j = (queue_num / 2); j > 0; j--){
			_min_heapify(harray, position, j, queue_num);
		}
		while(num_complete < num){
			pulled = _pull_min(harray, position, queue_num);
			
			for(k = 0;k < pulled->index_connected; k++){
				connected_name = pulled->connected[k];
				//connected = harray[connected_name];
				connected = harray[position[pulled->name]];
				distance = _get_distance(pulled, connected);
				total_distance = pulled->distance + distance;
				if(total_distance < connected->distance){
					connected -> distance = total_distance;
					connected -> prev = pulled;
				}
			}

			if(pulled){
				printf("pulled-> name: %d, pulled-> distance: %d\n", pulled->name, pulled->distance);
			}
			//if(pulled->name == destination){
			//	printf("should be done: distance: %d",pulled->distance);
			//}
			num_complete++;
			queue_num--;

			for(j = (queue_num / 2); j > 0; j--){
				_min_heapify(harray, position, j, queue_num);
			}
		}

	//}	

	return EXIT_SUCCESS;
}	
