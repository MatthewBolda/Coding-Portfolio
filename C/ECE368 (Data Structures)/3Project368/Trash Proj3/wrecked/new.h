#ifndef NEW_H
#define NEW_H

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


typedef struct Head{
	Node* head;
	int num;
	int cons;
}Head;


int _get_next_num_from_file(FILE* fp);
int _get_distance(Node* start, Node* end);
void _get_list_nodes(FILE* fp, Head* header, Node** harray);
int _get_next_num_from_file(FILE* fp);
void _add_to_list(Head* header, Node* node, Node** harray);
void _fill_connected(FILE* fp, Head* header, Node** harray);
void _min_heapify(Node** harray, int* position, int index, int queue_num);
Node* _pull_min(Node** array, int* position, int size);
#endif
