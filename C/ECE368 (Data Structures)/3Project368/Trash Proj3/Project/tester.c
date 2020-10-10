#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Node{
	int name;
	int x;
	int y;
	int* connected;
	struct Node* next;
	int distance;
	int visited;
	struct Node* prev;
}Node;

typedef struct Queue{
	int front;
	int last;
	int size;
	unsigned int capacity;
	int* array;
}Queue;

int isQueueEmpty(Queue* queue){
	return (queue->size==0);
}

void _add_to_queue(Queue* queue, int adder){
	queue -> last = (queue -> last + 1) % queue -> capacity;
	queue -> array[queue-> last] = adder;
	queue -> size = queue->size + 1;
}

int _delete_from_queue(Queue* queue){
	int front = queue->array[queue->front];
	queue->front = (queue->front + 1) % queue-> capacity;
	queue->size = queue -> size -1;
	return front;
}

Queue* _create_queue(unsigned int capacity){
	Queue* queue = malloc(sizeof(Queue));
	queue -> capacity = capacity;
	queue -> size = 0;
	queue -> front = 0;
	queue -> last = -1;
	queue -> array = (int*) malloc(queue-> capacity * sizeof(int));
	return queue;
}

void _free_queue(Queue* queue){
	free(queue->array);
	free(queue);
}

int _get_next_num_from_file(FILE* fp){
	char c;
	c = fgetc(fp);
	if(c == 32){
		while(c==32){
			c = fgetc(fp);
		}
	}
	int num = 0;
	int temp;
	while((c!=32) && (c != '\n') && (c != EOF)){
		temp = atoi(&c);
		num = (num * 10) + temp;
		c = fgetc(fp);
	}
	return num;
}

void _get_hash_map(FILE* fp, Node** harray, int size){
	int index = 0;
	int name;
	int x;
	int y;
	while(index < size){
		name = _get_next_num_from_file(fp);
		x = _get_next_num_from_file(fp);
		y = _get_next_num_from_file(fp);

		Node* node = malloc(sizeof(Node));
		node -> x = x;
		node -> y = y;
		node 
}



