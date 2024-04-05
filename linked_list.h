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

Node* findNode(SortedLinkedList* list, size_t size_in_words); 
void printNode(Node* node); 
void printLinkedList(SortedLinkedList* list); 
Node* findNodeByPointer(SortedLinkedList* list, void* ptr); 

/*-------------------------FUNCTIONS/VARIABLES TO MANAGE THE HEAP-------------------------------*/

extern SortedLinkedList list;
extern int mem_alloc[MAX_NUMBER_OF_NODES];
extern Node node_alloc[MAX_NUMBER_OF_NODES];
extern void initialize(void); 
extern Node* allocateNode(void);
extern bool deAllocateNode(Node* node); 
extern void printAllocations(void); 
extern bool tryDeFragment(Node* prev, Node* current); //returns 0 if success. 

#endif