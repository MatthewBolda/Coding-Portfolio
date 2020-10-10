#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "new_adjacent.h"

//helper function to add nodes to list
void _add_to_list(Head* header, Node* node, Node**harray){
	Node* head = header-> head;
	int temp_name;
	Node* temp_node;
	//int inserted = 0;
	if(head == NULL){
		header -> head = node;
	}
	else{
		temp_name = node->name;
		temp_node = harray[temp_name-1];
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
void _get_list_nodes(FILE* fp, Head* header, Node** harray, int* position){
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
		node -> next_in_path = NULL;
		//if ( name > 87600){
		//	printf("test");
		//}
		
		_add_to_list(header,node,harray);
		harray[index] = node;
		position[index] = name;
		index++;

	}
}

//function to find which are vertices are connected
void _fill_connected(FILE* fp, Head* header, Node** harray){
	int starter = _get_next_num_from_file(fp);
	int connected = _get_next_num_from_file(fp);
	Node* temp1 =harray[starter];// header-> head;//harray[starter]
	Node* temp2 =harray[connected];// header-> head;//harray[connected];
	/*
	while(temp1 -> name != starter || temp2 -> name != connected){
		if(temp1 -> name != starter){
			temp1 = temp1 -> next;
		}
		if(temp2 -> name != connected){
			temp2 = temp2 -> next;
		}
	}
	*/
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
		//if(temp -> connected){
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

/*
//function to implement my version of dijkstras algorithm
void _get_dist_table(Head* header, Queue* queue,Node** harray){//,Node* destination){//
	//need head, queue
	while(!isQueueEmpty(queue)){
		int checker = _delete_from_queue(queue);

		Node* temp = harray[checker];
		
		if (temp->visited == 0){
			temp->visited = 1;
			//already remove from queue
			int i;
			Node* next_node = header->head;
			int temp_name;
			int distance;
			for(i=0; i< (temp->index_connected); i++){
				temp_name = temp->connected[i];
				next_node = harray[temp_name];
				if (next_node -> visited == 0){
					//add name to queue
					_add_to_queue(queue, temp_name);
				}
				distance = _get_distance(temp, next_node);
				
				if ((next_node -> distance == -1)){// || (next_node -> distance > distance)){
					next_node -> prev = temp;
					//if(temp-> next_in_path == NULL){ printf("tester\n\n\n\n\n\n\n");}
					//added
					temp -> next_in_path = next_node;
					//
					next_node -> distance = (next_node -> prev -> distance) + distance +1;
					next_node -> dist_to_prev = distance;
				}
				
				//next_node->total_distance = next_node-> total_distance;	
				if (next_node -> distance > (temp->distance + distance)){
					//next_node->total_distance = -1;
					next_node -> prev = temp;
					//added
					temp -> next_in_path = next_node; // i think i need to refresh all
					//
					if (temp ->distance == -1){
						temp -> distance = temp->distance + 1;
					}
					next_node -> distance = (next_node -> prev -> distance) + distance;
					next_node -> dist_to_prev = distance;
				}
				next_node = header->head;
			}
		}
	}
}
*/
///*
//function to print results in the way needed for assignment
void _print_result(Head* header, Node* destination){
	
	
	//NO LONGER DIST TO PREV
	//JUST DIST
	
	
	//int distance = destination -> distance;
	int distance = 0;
	int destination_name = destination -> name;
	//Node* temp;
	int counter = 1;
	int* array = malloc(header->num * sizeof(int));
	int prev;
	while(destination-> prev != NULL){
		distance = distance + destination -> dist_to_prev;
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
//*/
//function to reset the distance and visited flags for next query
void _clear_distances(Head* header){
	Node* temp = header -> head;
	while(temp != NULL){
		temp -> prev = NULL;
		temp -> visited = 0;
		temp -> distance = -1;
		temp = temp -> next;
	}
}
/*
Heap* _create_heap(Node** array,int* position, int num){
	Heap* heap = malloc(sizeof(Heap));
	heap -> current_size = num;
	heap -> original_size = num;
	heap -> array = array;
	heap -> position = position;

	return heap;
}
*/
Heap* _create_heap(int num){
	Heap* heap = malloc(sizeof(Heap));
	heap -> current_size = 0;
	heap -> original_size = num;
	heap -> array = (Node**)malloc(num* sizeof(Node*));				// MIGHT BE WRONG wrong
	//heap -> position = position;

	return heap;
}
void _destroy_heap(Heap* heap){
	int i=0;
	while(heap->array[i] != NULL){
		free(heap->array[i]);
		i++;
	}
	//for(i = 0; i<heap->current_size; i++){
	//printf("current size %d\n\n\n\n\n",heap->current_size);
		//free(heap->array[i]->connected);
		//free(heap->
		free(heap->array[i]);
	//}
	free(heap->array);
	free(heap);
}

Node* _pull_min(Heap* heap){
	if(heap->current_size == 0){
		printf("heap was empty");
		return NULL;
	}
	Node* first = heap->array[0];
	heap->array[0] = heap->array[heap->current_size -1];
	
	//does position change
	heap-> current_size = heap -> current_size - 1;
	_min_heapify(heap, 0);
	return first;

}

void _min_heapify(Heap* heap, int index){
	int parent = index;
	int l_child = (2*index) + 1;
	int r_child = (2*index) + 2;
	
	if(l_child < heap -> current_size){
		if(heap->array[l_child]->distance < heap->array[parent] -> distance&& heap->array[l_child] -> distance != -1){
			parent = l_child;
		}
	}
	if(r_child < heap -> current_size){
		if(heap->array[r_child]->distance < heap->array[parent] -> distance && heap->array[r_child] -> distance != -1){
			parent = r_child;
		}
	}
	if(parent != index){
		Node* parent_node = heap->array[parent];
		Node* index_node = heap->array[index];

		heap->position[parent_node->name] = index;
		heap->position[index_node->name] = parent;
		
		heap->array[parent] = index_node;
		heap->array[index] = parent_node;

		_min_heapify(heap,parent);
		/*
		Node* temp = heap->array[parent];
		heap->array[parent] = heap->array[index];
		heap->array[index] = temp;

		heap->position[array[parent]-> name] = index;
		heap->position[array[index]->name] = parent;
		_min_heapify(heap,parent);
		*/
	}

}

Node* _insert_node(Node* vertex, int dist, Node* prev){
	Node* new_node = malloc(sizeof(Node));
	
	new_node -> prev = prev;
	new_node -> distance = dist;
	new_node -> x = vertex -> x;
	new_node -> y = vertex -> y;
	new_node -> connected = vertex -> connected;
	new_node -> index_connected = vertex -> index_connected;
	new_node -> name = vertex -> name;
	
	return new_node;
}



void _dijkstra(Node** harray,int source, int destination, int size, Head* header){
	Node* source_node = harray[source];
	source_node -> distance = 0;					//this needs to reset
	Heap* heap = _create_heap(size);

	heap->array[0] = _insert_node(source_node,0,NULL);
	heap->current_size = heap->current_size+1;
	//insert source_node into heap
	int i;
	Node* temp;
	int temp_name;
	int distance;
	int done = 0;
	int index = 1;
	Node* pulled;
	while(heap->current_size != 0 && done != 1){
		pulled = _pull_min(heap);
		if(pulled-> name == destination){
			done = 1;
		}
		else{
			for(i=0; i < (pulled->index_connected); i++){
				temp_name = pulled->connected[i];
				temp = harray[temp_name];
				distance = _get_distance(pulled, temp);
				heap->array[index] = _insert_node(temp,distance,pulled);
				index++;
						
			}
				_min_heapify(heap,index);
		}
		


	}
	_print_result(header, pulled);			// wrong needs to stop at destination now maybe not
	source_node -> distance = -1; 			
	//_clear_distances(header);					//maybe not needed
	_destroy_heap(heap);
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
	int position[num];
	_get_list_nodes(fp, header, hashArray,position);
		
	//printf("done _get_list_nodes");//this is the slow one

	int i;
	for(i = 0; i<(header->cons); i++){
		_fill_connected(fp, header,hashArray);
	}
	fclose(fp);
	fp = fopen(argv[2], "r");
	
	//Heap* heap = _create_heap(hashArray, position ,num);
	//if(heap){}	
	int num_of_queries = _get_next_num_from_file(fp);
	int source;
	int destination;
	//Queue* queue;// = _create_queue(num); 
	//Node* destination_node;
	//Node* temp;
	for(i = 0; i<num_of_queries; i++){
		source = _get_next_num_from_file(fp);
		destination = _get_next_num_from_file(fp);
		_dijkstra(hashArray,source,destination,num,header);
		_clear_distances(header);					//maybe not needed
		//hashArray[source]->distance = 0;
		//temp = hashArray[0];
		//hashArray[0] = hashArray[source];
		//hashArray[source] = temp;
		//_min_heapify(heap, 0);
		//_dijkstra(heap);
		if (i != (num_of_queries - 1)){
			printf("\n");
		}

	}
/*
	for(i = 0; i<num_of_queries; i++){
		start_found = 0;
		end_found = 0;
		queue = _create_queue(num); 
		temp2 = header->head;
		source = _get_next_num_from_file(fp); // first is 6
		_add_to_queue(queue, source);
		destination = _get_next_num_from_file(fp); // end is 23
		if( num >= source && num >= destination){
			start_found = 1;
			end_found = 1;
		}
		destination_node = hashArray[destination];
		if(end_found == 1 && start_found == 1){
			_get_dist_table(header, queue, hashArray); 
			if(destination_node -> distance == -1){
				printf("INF\n");
				printf("%d %d", source, destination);
			}
			else{
				_print_result(header, destination_node);
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
		_free_queue(queue);
		
	}

	fclose(fp);
	_free_everything(header);
	*/
	fclose(fp);
	_free_everything(header);
	
	return EXIT_SUCCESS;
}

