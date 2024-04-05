/*
Although this may seem like an infrastructure header file, actually this is a specific
implementation for the project. The node structure is specifically designed to hold 
a start offset for the memory location and size of the memory (could be in bytes... )
*/

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_
#define MAX_CONCURRENT_ALLOCATIONS 20
#define MAX_MEMORY_TO_SUPPORT_HEAP 64000
#define MAX_NUMBER_OF_NODES 20

#include <stdint.h>
#include <stdbool.h>

/*-------------------------LINKED LIST DECLARATION AND FUNCTIONS-------------------------------*/

typedef struct Node Node;

struct Node {
    uintptr_t* start;
    size_t size_in_words;
    Node* next; 
    Node* previous; 
    int allocated_index; 
    bool is_allocated; 
};  

typedef struct {
    Node* head; 
    int count_allocated; 
    int count_total; 
    int count_de_allocated; 
} SortedLinkedList; 

Node* FindNode(SortedLinkedList* list, size_t size_in_words); 
void MarkNodeAsAllocated(Node* node); 
void print_node(Node* node); 
void print_linked_list(SortedLinkedList* list); 
Node* find_node_by_pointer(SortedLinkedList* list, void* ptr); 

/*-------------------------FUNCTIONS/VARIABLES TO MANAGE THE HEAP-------------------------------*/

extern SortedLinkedList list;
extern int mem_alloc[MAX_NUMBER_OF_NODES];
extern Node node_alloc[MAX_NUMBER_OF_NODES];
extern void print_list(); 

extern void InitializeAllocators(void); 
extern Node* alloc_node(void);
extern void de_alloc_node(Node* node); 
extern void print_allocations(void); 

#endif