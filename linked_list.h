/*
Although this may seem like an infrastructure header file, actually this is a specific
implementation for the project. The node structure is specifically designed to hold 
a start offset for the memory location and size of the memory (could be in bytes... )
*/

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#define MAX_CONCURRENT_ALLOCATIONS 1024
#define MAX_MEMORY_TO_SUPPORT_HEAP 64000

extern void* allocation_array[MAX_CONCURRENT_ALLOCATIONS];

#include <stdbool.h>

typedef struct {
    int start_offset;
    size_t size;
    struct Node* next; 
    struct Node* previous; 
} Node; 

typedef struct {
    Node* head; 
    int count; 
} SortedLinkedList; 

void AddNode(SortedLinkedList* list, void* value); 
void RemoveNode(SortedLinkedList* list, Node* node); 
Node* FindNode(SortedLinkedList* list, Node* node);

#endif