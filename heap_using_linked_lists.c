#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./heap.h"
#include "./linked_list.h"
#include <assert.h>

Node* initialize_head_node(void); 

uintptr_t heap[HEAP_CAP_WORDS] = {0};
const uintptr_t *stack_base = 0;
size_t convert_bytes_to_words_size(size_t size_in_bytes);

/*
The intention is as below:
1) Have a linked list to manage heap allocations - having linked list sorted will make the search 
O(log2(n)) - this will keep Allocated Slots. Important note is linked list will hold empty spaces...
2) Important Notes about the linked list:
- Obviously since we are re-writing malloc function - we need to have a way to initialize the 
linked list - as a start we will start with an array. As second stage, we would allocate the
linked list in the allocated memory for the heap. 
- How are we going to track the allocation of the linked list - since it will be a list 
implemented on a memory array. We will need a fixed size array (int/bool) to track the allocation
status of the list. -> by index (again considering this is implemented on the memory array)
3) When allocating memory segments (this would remove/modify a linked list node), 
there can be 2 strategies to follow: 
- Return first available time slot same or greater than required
- Return the time slot that would optimally fit the request -> this will not be implemented, as
would require O(n) with planned data structures. 
4) When free'ing memory slots (this would add/modify a node)
- a new node would be added to the linked list. 
- at first stage the node will be added
- as second stage, adjacent nodes would be checked, if possible those will be merged. 

IMPORTANT NOTE: AIM IS TO MOVE THE DATA STRUCTURE ALLOCATION TO PART OF THE HEAP 
ARRAY - SO WE HAVE AN ALTOGERHER ALLOCATION MANAGEMENT...
THIS WILL BE DONE AS SECOND STAGE...*/

/* ------------------------------ HEAP FUNCTIONS --------------------------------*/
void *heap_alloc(size_t size_bytes)
{
    if(list.count >= MAX_CONCURRENT_ALLOCATIONS - 1)
        return NULL; 

    size_t size_in_words = convert_bytes_to_words_size(size_bytes);
    
    Node* node = FindNode(&list, size_in_words); 

    if(node == NULL)
        return NULL; 
    
    if(node->size_in_words == size_in_words)
    {
        MarkNodeAsAllocated(node); 
        return node; 
    }

    //here it's clear that node size is greater than required
    //thus we need to split it into two
    //we would need to split the node into 2... 

    Node* emptyNode = alloc_node();

    emptyNode->is_allocated = true;
    emptyNode->next = node->next; 
    emptyNode->previous = node;
    emptyNode->size_in_words = size_in_words; 
    emptyNode->start = node->start + (node->size_in_words - size_in_words); 

    node->size_in_words = node->size_in_words - size_in_words; 
    node->next = emptyNode; 
    list.count++; 

    return emptyNode->start; 
}

size_t convert_bytes_to_words_size(size_t size_in_bytes)
{
    size_t words = (size_t)size_in_bytes / (sizeof(uintptr_t));

    if(size_in_bytes % sizeof(uintptr_t) != 0)
        words++;

    return words;  
}

void heap_free(void *ptr)
{
    uintptr_t* casted = (uintptr_t*)ptr; 

    int index = casted - list.head->start; 

    node_alloc[index].is_allocated = false; 

    //to-do --- combine with adjacent nodes...

    mem_alloc[index] = 0;
}
void heap_collect() {} //at this stage this will not be implemented, not that necessary. 

/* ------------------------------ LINKED LIST FUNCTIONS --------------------------------*/

Node* FindNode(SortedLinkedList* list, size_t size_in_words)
{
    Node* temp = list->head; 
    while(temp != NULL)
    {
        if(!temp->is_allocated && temp->size_in_words >= size_in_words)
            return temp; 
        
        temp = temp->next; 
    }
    return temp; 
}

void MarkNodeAsAllocated(Node* node)
{
    node->is_allocated = true; 
}

/*-------------------------FUNCTIONS/VARIABLES TO MANAGE THE HEAP-------------------------------*/
//this is the sorted linked list for 

SortedLinkedList list = {.head = NULL, .count = 0}; 
int mem_alloc[MAX_CONCURRENT_ALLOCATIONS] = {0};
Node node_alloc[MAX_CONCURRENT_ALLOCATIONS]  = 
{
    {
        .is_allocated=false, 
        .next = NULL,
        .previous = NULL,
        .size_in_words = 0,
        .start = NULL, 
        .allocated_index = 0
    }
};
void InitializeAllocators(void)
{
    for(int i = 0; i < MAX_CONCURRENT_ALLOCATIONS; i++)
    {
        node_alloc[i].allocated_index = i; 
    }
    list.head = initialize_head_node(); 
}

Node* initialize_head_node(void)
{
    node_alloc[0].size_in_words = HEAP_CAP_WORDS;
    node_alloc[0].start = &heap[0]; 
    return &node_alloc[0];
}

Node* alloc_node(void)
{
    int i = 0; 
    for(; i<MAX_CONCURRENT_ALLOCATIONS; i++)
    {
        if(mem_alloc[i] == 0) //corresponding node is free
        {
            break; 
        }
    }

    if(i == MAX_CONCURRENT_ALLOCATIONS)
        return NULL; 

    node_alloc[i].allocated_index = i;
    mem_alloc[i] = 1; //mark node as allocated

    return &node_alloc[i];  
}

void de_alloc_node(Node* node)
{
    assert(node != NULL && node->allocated_index >= 0 && node->allocated_index < MAX_CONCURRENT_ALLOCATIONS); 

    mem_alloc[node->allocated_index] = 0; 
}