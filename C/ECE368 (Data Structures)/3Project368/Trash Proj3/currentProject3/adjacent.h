#ifndef ADJACENT
#define ADJACENT

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

typedef struct HeapNode{
  int distance;
  Node* vertex;
  struct HeapNode * left;
  struct HeapNode * right;
}HeapNode;

typedef struct Heap{
  int current_size;
  int original_size;
  struct HeapNode ** array;
}Heap;

int isQueueEmpty(Queue* queue);
void _add_to_queue(Queue* queue, int adder);
int _delete_from_queue(Queue* queue);
Queue* _create_queue(unsigned int capacity);
void _free_queue(Queue* queue);
void _add_to_list(Head* header, Node* node, Node** harray);
int _get_next_num_from_file(FILE* fp);
void _get_list_nodes(FILE* fp, Head* header, Node** harray);
void _fill_connected(FILE* fp, Head* header, Node** harray);
void _free_everything(Head* header);
//void _free_everything(Node** harray, int num);
int _get_distance(Node* start, Node* end);

void _get_dist_table(Head* header, Queue* queue,Node** harray);
//void _get_dist_table(Head* header, Head* header_queue,Node** harray, int destination);

void _print_result(Head* header, Node* destination);
void _clear_distances(Head* header);
//void _clear_distances(Node** harray, int num);

//Node* _get_node_queue(Head* header_queue);
//void _insert_in_queue(Head* header_queue, Node* insert);

#endif
