#ifndef HUFF
#define HUFF

typedef struct LinkedList {
	struct ListNode * head;
	int size;
}LinkedList;

typedef struct ListNode {
	int ASCII;
	int freq;
	int code[100];
	int code_index;
	struct ListNode * next;
}ListNode;

typedef struct HeapNode {
	int ASCII;
	int freq;
	struct HeapNode * left;
	struct HeapNode * right;
}HeapNode;

typedef struct Heap {
	int current_size;
	int original_size;
	struct HeapNode ** array;
}Heap;
/*
typedef struct TableHead{
	struct TableNode * head;
}TableHead;
typedef struct Table{
	int code; //this is not right
	int ASCII;
	struct Table * next;
}Table;
*/

// used to create character freq array
void _add_to_list(LinkedList *, ListNode *);
unsigned long long *  _data_count_array(FILE*,unsigned long long *);
LinkedList * _data_linked_list(unsigned long long *);
//used to create heap and heap tree
Heap * _create_heap(LinkedList*);
HeapNode* _create_heap_tree(Heap*);
void _heapify(Heap*, int);
//used to get huffman codes
//int _get_height(HeapNode*);
void _get_Codes(HeapNode*, int *, int, LinkedList*);
//used to convert file to huffman
void _convert_to_huff(FILE *,FILE *, LinkedList*, unsigned long long *);
ListNode*   _get_code(char, LinkedList* /*, char**/);

//used to convert file to unhuff
void _convert_to_unhuff(FILE*,FILE*, HeapNode*);


void _free_tree(HeapNode*);
void _free_list(ListNode*);

#endif
