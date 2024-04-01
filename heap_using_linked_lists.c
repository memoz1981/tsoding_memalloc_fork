#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./heap.h"
#include "./linked_list.h"
#include <assert.h>

uintptr_t heap[HEAP_CAP_WORDS] = {0};
const uintptr_t *stack_base = 0;

/*
The intention is as below:
1) Have a linked list to manage heap allocations - having linked list sorted will make the search 
O(log2(n)) - this will keep Allocated Slots. 
2) Important Notes about the linked list:
- Obviously since we are re-writing malloc function - we need to have a way to initialize the 
linked list - as a start we will start with an array. As second stage, we would allocate the
linked list in the allocated memory for the heap. 
- How are we going to track the allocation of the linked list - since it will be a list 
implemented on a memory array. We will need a fixed size array (int/bool) to track the allocation
status of the list. -> by index (again considering this is implemented on the memory array)
3) When adding / removing memory segments - we would need to check the linked list for current status
4) When removing (allocating a memory slot) -> 
a) first available slot will be returned
b) if the slot size is larger than required, slot start will be adjusted. 
5) When adding (re-instating the memory slot back to heap) -> 
a) We need to find the correct location (O(log(n)))
b) 
*/

/* ------------------------------ HEAP FUNCTIONS --------------------------------*/



//Define 

void *heap_alloc(size_t size_bytes);
void heap_free(void *ptr);
void heap_collect();

/* ------------------------------ LINKED LIST FUNCTIONS --------------------------------*/

void AddNode(SortedLinkedList* list, Node* node)
{
    assert(list != NULL); 
    assert(node != NULL); 

    if(list->head == NULL)
    {
        list->head = node; 
        list->tail = node; 
        list->count++; 
        return; 
    }

    Node* temp = list->head; 

    while(temp != NULL)
    {
        if(temp->next == NULL)
            break; 

        if(temp->next->start_offset > node->start_offset)
            break; 
    }

    node->previous = temp; 
    node->next = temp->next; 
    temp->next = node; 

    list->count++; 
}

void RemoveNode(SortedLinkedList* list, Node* node); 


/*-------------------------FUNCTIONS/VARIABLES TO MANAGE THE HEAP-------------------------------*/
SortedLinkedList list = {.head = NULL, .tail = NULL, .count = 0}; 
int mem_alloc[MAX_CONCURRENT_ALLOCATIONS] = {0};
Node node_alloc[MAX_CONCURRENT_ALLOCATIONS] = {{.size=0, .start_offset=0, .next=NULL, .previous=NULL}};

int return_first_free_index(void)
{
    for(int i=0; i<MAX_CONCURRENT_ALLOCATIONS; i++)
    {
        if(mem_alloc[i]==1)
            return i; 
    }

    return -1; 
}

void mark_index_allocated(int index)
{
    assert(index >=0 && index < MAX_CONCURRENT_ALLOCATIONS); 
    assert(mem_alloc[index] == 0); 
    mem_alloc[index] = 1; 
} 