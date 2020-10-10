
#ifndef NEW_ADJACENT
#define NEW_ADJACENT


typedef struct Heap{
	int current_size;
	int original_size;
	int* position;
	struct Node** array;
}Heap;
//this is the struct that does the most work
//all the data for the vertices and their edges
typedef struct Node{
	int name;
	int x;
	int y;
	int* connected;
	int index_connected;
	struct Node* next;
	struct Node* next_in_path;
	int distance;
	int dist_to_prev;
	int visited;
	struct Node* prev;
}Node;

//this struct is for pointing to head of the vertices list
typedef struct Head{
	Node* head;
	int num;
	int cons;
}Head;

void _add_to_list(Head* header, Node* node, Node** harray);
int _get_next_num_from_file(FILE* fp);
void _get_list_nodes(FILE* fp, Head* header, Node** harray, int* position);
void _fill_connected(FILE* fp, Head* header, Node** harray);
void _free_everything(Head* header);
int _get_distance(Node* start, Node* end);
//void _get_dist_table(Head* header, Queue* queue,Node** harray);
void _print_result(Head* header, Node* destination);
void _clear_distances(Head* header);
Heap* _create_heap(int num);
void _destroy_heap(Heap* heap);
void _min_heapify(Heap* heap, int index);
Node* _pull_min(Heap* heap);
void _dijkstra(Node** harray,int source, int destination,int size, Head* header);
Node* _insert_node(Node* vertex, int dist, Node* prev);

#endif
