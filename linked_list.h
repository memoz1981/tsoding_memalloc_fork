/*
Although this may seem like an infrastructure header file, actually this is a specific
implementation for the project. The node structure is specifically designed to hold 
a start offset for the memory location and size of the memory (could be in bytes... )
*/

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_
#define MAX_CONCURRENT_ALLOCATIONS 1024
#define MAX_MEMORY_TO_SUPPORT_HEAP 64000

/*-------------------------LINKED LIST DECLARATION-------------------------------*/

typedef struct Node Node;

struct Node {
    int start_offset;
    size_t size;
    Node* next; 
    Node* previous; 
};  

typedef struct {
    Node* head; 
    Node* tail; 
    int count; 
} SortedLinkedList; 

void AddNode(SortedLinkedList* list, Node* node); 
void RemoveNode(SortedLinkedList* list, Node* node); 
void AddNode(SortedLinkedList* list, Node* node); 
void RemoveNode(SortedLinkedList* list, Node* node); 

/*-------------------------FUNCTIONS/VARIABLES TO MANAGE THE HEAP-------------------------------*/

extern SortedLinkedList list;
extern int mem_alloc[MAX_CONCURRENT_ALLOCATIONS];
extern Node node_alloc[MAX_CONCURRENT_ALLOCATIONS];

extern int return_first_free_index(void);
extern void mark_index_allocated(int index); 

#endif