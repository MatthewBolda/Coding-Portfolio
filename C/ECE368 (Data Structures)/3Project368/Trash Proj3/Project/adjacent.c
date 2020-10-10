#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

//this is the struct that does the most work
//all the data for the vertices and their edges
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

//this struct is for implementation of a queue
//needed for breadth first searching
typedef struct Queue{
	int front;
	int last;
	int size;
	unsigned int capacity;
	int* array;	
}Queue;

//this struct is for pointing to head of the vertices list
typedef struct Head{
	Node* head;
	int num;
	int cons;
}Head;

int isQueueEmpty(Queue* queue){
	return (queue-> size == 0);
}

void _add_to_queue(Queue* queue, int adder){
	queue -> last = (queue->last + 1) % queue-> capacity;
	queue ->array[queue->last] = adder;
	queue->size = queue->size + 1;
}

int _delete_from_queue(Queue* queue){
	int front = queue->array[queue->front];
	queue -> front = (queue->front + 1)% queue->capacity;
	queue -> size = queue-> size - 1;
	return front;
}


Queue* _create_queue(unsigned int capacity){
	Queue* queue = malloc(sizeof(Queue));
	queue->capacity = capacity;
	queue->size = 0;
	queue->front = 0;
	queue->last = -1;
	queue->array = (int*) malloc(queue->capacity* sizeof(int));   //NOT RIGHT
	return queue;
}

void _free_queue(Queue* queue){
	free(queue->array);
	free(queue);
}



//helper function to add nodes to list
void _add_to_list(Head* header, Node* node, Node** harray){
	Node* head = header-> head;
	//int inserted = 0;
	if(head == NULL){
		header -> head = node;
	}
	//else{	
	//	int temp_name = node->name;
	//	Node* temp_node = harray[temp_name-1];
	//	temp_node -> next = node;
	//}
	/*else{
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
	}*/
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
void _free_everything(Node** harray, int num){	//TODO need to change to harray!
	
	int i;
	for(i=0; i < num; i++){
		free(harray[i]);
	}
	/*
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
	*/
}

//trivial function to get distance between two vertices
int _get_distance(Node* start, Node* end){
	int distance = pow((pow((start->x - end -> x),2)+pow((start->y - end->y),2)),0.5);
	return distance;
}

Node* _get_node_queue(Head* header_queue){
	Node* temp = header_queue -> head;
	header_queue ->head = temp -> next;
	while (temp->visited == 1){
		temp = temp -> next;
		header_queue -> head = temp;
	}
	return temp;

}
void _insert_in_queue(Head* header_queue, Node* insert){
	Node* checker = header_queue -> head;
	if (header_queue -> head == NULL){
		header_queue -> head = insert;
	}
	else{
		Node* temp;
		while(checker->distance < insert->distance && checker -> next != NULL){
			checker = checker->next;
		}
		temp = checker -> next;
		checker -> next = insert;
		insert -> next = temp;
	}
}



//function to implement my version of dijkstras algorithm
////////void _get_dist_table(Head* header, Queue* queue,Node** harray){
void _get_dist_table(Head* header, Head* header_queue, Node** harray, int destination){
	//need head, queue
	int done = 0;
	Node* next_node;
	//while(!isQueueEmpty(queue)){
	while(done == 0){
		//int checker = _delete_from_queue(queue);
		////Node* temp = harray[checker];
		//Node* temp = header_queue -> head;
		Node* temp = _get_node_queue(header_queue);
		if(temp->name == destination){
			done = 1;
		}
		//TODO header_queue	
		if (temp->visited == 0){
			temp->visited = 1;
			//already remove from queue
			int i;
			//Node* next_node = header->head;
			int temp_name;
			int distance;
			for(i=0; i< (temp->index_connected); i++){
				temp_name = temp->connected[i];
				next_node = harray[temp_name];
				distance = _get_distance(temp, next_node);
				
				if ((next_node -> distance == INT_MAX)){// || (next_node -> distance > distance)){
					next_node -> prev = temp;
					next_node -> distance = (next_node -> prev -> distance) + distance +1;
				}
				
				//next_node->total_distance = next_node-> total_distance;	
				if (next_node -> distance > (temp->distance + distance)){
					//next_node->total_distance = -1;
					next_node -> prev = temp;
					if (temp ->distance == -1){
						temp -> distance = temp->distance + 1;
					}
					next_node -> distance = (next_node -> prev -> distance) + distance;
				}
				if (next_node -> visited == 0){
					//add name to queue
					////_add_to_queue(queue, temp_name);
					_insert_in_queue(header_queue, next_node);
					//TODO header_queue
				}
			}
		}
	}
}

//function to print results in the way needed for assignment
void _print_result(Head* header, Node* destination){
	int distance = destination -> distance;
	int destination_name = destination -> name;
	//Node* temp;
	int counter = 1;
	int* array = malloc(header->num * sizeof(int));
	int prev;
	while(destination-> prev != NULL){
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
//void _clear_distances(Head* header){
void _clear_distances(Node** harray, int num){
	int i;
	for(i=0; i < num; i++){
		harray[i]-> distance = INT_MAX;
		harray[i] -> visited = 0;
		harray[i] -> prev = NULL;
	}
	/*
	Node* temp = header -> head;
	while(temp != NULL){
		temp -> prev = NULL;
		temp -> visited = 0;
		temp -> distance = -1;
		temp = temp -> next;
	}
	*/
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
	//Node* temp2;// = header-> head;
	////Queue* queue;// = _create_queue(num); 
	int start_found;
	int end_found;
	Node* destination_node;
		
//	Head* header_queue = malloc(sizeof(Head)); //size of connections???????

	Node* sourcer;
	//Node* adder;
	//Node* ender;
	for(i = 0; i<num_of_queries; i++){
		start_found = 0;
		end_found = 0;
		Head* header_queue = malloc(sizeof(Head)); //size of connections???????
		////queue = _create_queue(num); 
		
		//TODO header_queue
		


		////temp2 = header->head;
		source = _get_next_num_from_file(fp); // first is 6
		////_add_to_queue(queue, source);
		
		//TODO header_queue
		sourcer = hashArray[source];
		sourcer-> distance = 0;
		_insert_in_queue(header_queue,sourcer);
		
		destination = _get_next_num_from_file(fp); // end is 23
		if( num >= source && num >= destination){
			start_found = 1;
			end_found = 1;
		}	
		destination_node = hashArray[destination];
		if(end_found == 1 && start_found == 1){
			////_get_dist_table(header, queue, hashArray); 
			_get_dist_table(header, header_queue, hashArray, destination); 
			//TODO change to header->queue
			
			
			if(destination_node -> distance == -1){
				printf("INF\n");
				printf("%d %d", source, destination);
			}
			else{
				_print_result(header, destination_node);
			}
			////_clear_distances(header);
			_clear_distances(hashArray, num);
		}
		else{
			printf("INF\n");
			printf("%d %d", source, destination);
		}
		if (i != (num_of_queries - 1)){
			printf("\n");
		}
		////_free_queue(queue);
		free(header_queue);
		
	}
	//free(header_queue);
	fclose(fp);
	//_free_everything(header);
	_free_everything(hashArray, num);
	
	
	return EXIT_SUCCESS;
}

