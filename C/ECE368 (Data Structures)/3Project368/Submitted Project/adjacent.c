#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "adjacent.h" 

Heap * _create_heap(Node* insert, int num){
	Heap* heap = malloc(sizeof(Heap));
	heap -> current_size = 1;
	heap -> original_size = num;
	heap -> max_size = 1; //TODO make sure this is right
	heap -> array = malloc(sizeof(HeapNode*) * num);
	HeapNode* node = malloc(sizeof(HeapNode*));
	//node->vertex = &insert;
	node->name = insert->name;
	node -> distance = 0;
	node -> prev_name = insert->name;
	heap->array[0] = node;
	
	return heap;

}

void _find_position(Heap* heap, int index){
	int parent = (index -1) / 2;

	if( heap->array[parent] != NULL){
		if (heap->array[parent]->distance > heap->array[index]->distance){
			HeapNode* temp = heap->array[index];
			heap->array[index] = heap->array[parent];
			heap->array[parent] = temp;

			_find_position(heap, index);
		}
	}
}

void _insert_in_heap(Heap* heap, Node* insert){
	HeapNode* node = malloc(sizeof(HeapNode*));
	node -> distance = insert-> distance;
	//node -> vertex = insert;
	node->name = insert->name;
	node->prev_name = insert->prev->name;

	//heap->array[heap ->current_size] = insert;
	heap->array[heap ->current_size] = node;
	heap-> current_size = heap->current_size + 1;
	heap-> max_size = heap->max_size + 1;
	
	_find_position(heap, heap->current_size - 1);
}

void _heapify(Heap* heap, int index){
	int parent = index;
	int left_child = (2*index) + 1;
	int right_child = (2*index) + 2;
	if(left_child < heap -> current_size){
		if( heap->array[left_child] -> distance < heap->array[parent]->distance){
			parent = left_child;
		}
	}
	if(right_child < heap-> current_size){
		if(heap->array[right_child] -> distance < heap->array[parent]->distance){
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

HeapNode* _remove_min(Heap* heap){
	HeapNode* smallest = heap->array[0];
	HeapNode* largest  = heap->array[heap->current_size - 1];
	heap->array[0] = largest;
	heap->current_size = heap->current_size - 1;

	_heapify(heap, 0);
	return smallest; //TODO, convert to node somehow
	
}

//helper function to add nodes to list
void _add_to_list(Head* header, Node* node, Node** harray){
	Node* head = header-> head;
	//int inserted = 0;
	if(head == NULL){
		header -> head = node;
	}
	else{	
		int temp_name = node->name;
		Node* temp_node = harray[temp_name-1];
		temp_node -> next = node;
	}
}

//code to get the next number from the file
int _get_next_num_from_file(FILE* fp){
	int num;
	fscanf(fp, "%d", &num);
	return num;
}

//function to create list with information
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
		node -> distance = INT_MAX;
		node -> visited = 0;
		//if ( name > 87600){
		//	printf("test");
		//}
		
		_add_to_list(header,node,harray);
		harray[index] = node;
		index++;

	}
}

//function to find which are vertices are connected
void _fill_connected(FILE* fp, Head* header, Node** harray){
	int starter = _get_next_num_from_file(fp);
	int connected = _get_next_num_from_file(fp);
	Node* temp1 =harray[starter];
	Node* temp2 =harray[connected];
	temp1-> connected[temp1->index_connected] = connected;
	temp1-> index_connected++;
	temp2-> connected[temp2->index_connected] = starter;
	temp2-> index_connected++;

}

//function to clean up all the mallocs and free them
void _free_everything(Head* header){
	Node* head = header-> head;
	Node* temp = header ->head;
	while(head != NULL){
		temp = head;
		head = head->next;
		free(temp->connected);
		//}
		free(temp);
	}
	free(header);
}

//trivial function to get distance between two vertices
int _get_distance(Node* start, Node* end){
	int distance = pow((pow((start->x - end -> x),2)+pow((start->y - end->y),2)),0.5);
	return distance;
}

void _get_dist_table(Head* header, Heap* heap,Node** harray,int destination){
	int done = 0;
	int j;
	int heap_size = 0;
	HeapNode* free_array[header->cons];

	while(heap->current_size != 0 && done == 0){
		for(j=0; j<heap->current_size / 2;j++){
			_heapify(heap,j);
		}
		HeapNode* checker = _remove_min(heap);
		free_array[heap_size] = checker;
		heap_size++;
		Node* temp = harray[checker->name];
		if(temp->name == destination){
			done = 1;
			temp->visited = 1;
			if(checker -> distance < temp->distance){
				temp->distance = checker -> distance;
				temp->prev = harray[checker->prev_name];
			}
			//free(checker);
		}
		else if(temp-> visited == 0){
			temp->visited = 1;
			if(checker -> distance < temp->distance){
				temp->distance = checker -> distance;
				temp->prev = harray[checker->prev_name];
			}
			//free(checker);
			int i;
			int temp_name;
			Node* next_node;
			int distance;
			for(i=0;i < (temp->index_connected); i++){
				temp_name = temp->connected[i];
				next_node = harray[temp_name];
				distance = _get_distance(temp,next_node);
				if (distance == 0){
					
				}
				if (next_node -> visited == 0){
					next_node -> distance = temp -> distance + distance;
					next_node -> prev = temp;
					_insert_in_heap(heap,next_node);
				}
			}
		}
	}
	int k;
	for(k=0; k < heap_size; k++){
		free(free_array[k]);
	}

}	

//function to print results in the way needed for assignment
void _print_result(Head* header, Node* destination, int source){
	int distance = destination -> distance;
	int destination_name = destination -> name;
	//Node* temp;
	int counter = 1;
	int* array = malloc(header->num * sizeof(int));
	int prev;
	while(destination-> name != source){
		prev = destination -> prev -> name;
		array[counter] = prev;
		counter ++;
		destination = destination -> prev;
		//temp = destination -> prev;
	}
	int i;
	printf("%d\n", distance);
	for(i = 1; i < counter; i++){
		printf("%d ", array[counter-i]);
		//printf("%d", i);
	}
	printf("%d", destination_name);
	free(array);
}

//function to reset the distance and visited flags for next query
void _clear_distances(Head* header){
	Node* temp = header -> head;
	while(temp != NULL){
		temp -> prev = NULL;
		temp -> visited = 0;
		temp -> distance = INT_MAX; //TODO, change to INT_MAX
		temp = temp -> next;
	}
}

void _free_heap(Heap* heap){
	int size = heap->current_size;
	int i=0;
	/*
	while(heap->array[i] != NULL){
		HeapNode* temp = heap->array[i];
		free(temp);
		i++;
	}
	*/
	HeapNode* temp;
	for(i = 0; i < size; i++){
		if(heap->array[i] != NULL){	
			temp = heap->array[i];
			free(temp);
			heap->array[i] = NULL;
		}
	
	}
	//if(heap->array[i+1] != NULL){
	//	temp = heap->array[i+1];
	//	free(temp);
	//}
	free(heap->array);
	free(heap);
}

//main function for implementing everything
int main(int argc, char ** argv){
	FILE* fp = fopen(argv[1], "r");
	int num = _get_next_num_from_file(fp);
	int cons = _get_next_num_from_file(fp);
	
	Head* header = malloc(sizeof(Head));
	header-> num = num;
	header-> cons=cons;
	Node* hashArray[num];	
	_get_list_nodes(fp, header, hashArray);
	int i;
	for(i = 0; i<(header->cons); i++){
		_fill_connected(fp, header,hashArray);
	}
	fclose(fp);
	fp = fopen(argv[2], "r");
	
	int num_of_queries = _get_next_num_from_file(fp);
	int source;
	int destination;
	Node* temp2;// = header-> head;
	int start_found;
	int end_found;
	Node* destination_node;
	
	//num_of_queries = 0;			//for testing

	for(i = 0; i<num_of_queries; i++){
		start_found = 0;
		end_found = 0;
		temp2 = header->head;
		source = _get_next_num_from_file(fp);
		Heap* heap = _create_heap(hashArray[source], cons);
		
		destination = _get_next_num_from_file(fp);
		if( num >= source && num >= destination){
			start_found = 1;
			end_found = 1;
		}	
		destination_node = hashArray[destination];
		if(end_found == 1 && start_found == 1){
			_get_dist_table(header, heap, hashArray, destination);
			if(destination_node -> distance == INT_MAX){
				printf("INF\n");
				printf("%d %d", source, destination);
			}
			else{
				_print_result(header, destination_node,source);
			}
			_clear_distances(header);
		}
		else{
			printf("INF\n");
			printf("%d %d", source, destination);
		}
		if (i != (num_of_queries - 1)){
			printf("\n");
		}
		//TODO free heap
		_free_heap(heap);	
	}

	fclose(fp);
	_free_everything(header);
	
	
	return EXIT_SUCCESS;
}

