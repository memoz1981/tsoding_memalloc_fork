#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./heap.h"
#include "./linked_list.h"
#include <assert.h>
#include "logger.h"
#include <stdarg.h>

uintptr_t heap[HEAP_CAP_WORDS] = {0};
Node* initializeHeadNode(void); 
size_t convertBytesToWords(size_t size_in_byprint_allocationstes);

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
    //Validation 1 - size should be greater than zero
    if(size_bytes <= 0)
    {
        logToConsole(Information, "Invalid request, size should be greater than or equal to zero.\n"); 
        
        return NULL; 
    }

    //Validation 2 - we shouldn't have more than Max     
    if(list.count_allocated >= MAX_CONCURRENT_ALLOCATIONS)
    {
        printf("Currently there are %d allocations - we cannot have more than that\n", list.count_allocated);
        return NULL; 
    }

    size_t size_in_words = convertBytesToWords(size_bytes);
    
    Node* node = findNode(&list, size_in_words); 

    //Validation 3 - check if any node could be allocated
    if(node == NULL)
    {
        printf("Memory with requested size doesn't exit.\n");
        return NULL; 
    }

    printf("Found memory  in node below: \n"); 
    printNode(node); 
    printf("\n"); 

    //check for exast match
    if(node->size_in_words == size_in_words)
    {
        printf("Node size %d is same as requested %d\n", (int)node->size_in_words, (int)size_in_words); 
        node->is_allocated = true; 
        list.count_allocated++; 
        list.count_de_allocated--; 
        return node; 
    }

    //here it's clear that node size is greater than required
    //thus we need to split it into two
    //we would need to split the node into 2... 

    Node* emptyNode = allocateNode();

    if(emptyNode == NULL)
    {
        printf("Error during memory allocation"); 
        return NULL; 
    }

    printf("Will use following node to accommodate new allocation: \n");
    printNode(emptyNode); 
    printf("\n"); 

    emptyNode->is_allocated = true;
    
    emptyNode->next = node->next; 
    emptyNode->previous = node;
    emptyNode->size_in_words = size_in_words; 
    emptyNode->start = node->start + (node->size_in_words - size_in_words); 

    if(emptyNode->next != NULL)
        emptyNode->next->previous = emptyNode; 

    node->size_in_words = node->size_in_words - size_in_words; 
    node->next = emptyNode; 
    list.count_allocated++; 
    list.count_total++; 

    return emptyNode->start; 
}

size_t convertBytesToWords(size_t size_in_bytes)
{
    size_t words = (size_t)size_in_bytes / (sizeof(uintptr_t));

    if(size_in_bytes % sizeof(uintptr_t) != 0)
        words++;

    return words;  
}

void heap_free(void *ptr)
{
    Node* node = findNodeByPointer(&list, ptr); 

    if(node == NULL)
    {
        printf("Provided pointer is not valid. \n"); 
        return; 
    }

    if(deAllocateNode(node) == false)
    {
        printf("Failed to de-allocate the node\n"); 
        return; 
    }

    printf("De-allocating node:\n"); 
    printNode(node);
    printf("Previous: \n");
    printNode(node->previous);
    printf("Next node: \n"); 
    printNode(node->next); 
    printf("\n"); 

    //check previous node - if both are free -> can be combined (to prevent de-fragmentation)
    Node* prev = node->previous; 
    Node* next = node->next; 

    bool mergedWithPrevious = tryDeFragment(prev, node); 
    
    if(mergedWithPrevious)
        tryDeFragment(prev, next); 
    else
        tryDeFragment(node, next); 
}

void heap_collect() {} //at this stage this will not be implemented, not that necessary. 

/* ------------------------------ LINKED LIST FUNCTIONS --------------------------------*/

Node* findNode(SortedLinkedList* list, size_t size_in_words)
{
    Node* temp = list->head; 
    while(temp != NULL)
    {
        if(!temp->is_allocated && temp->size_in_words >= size_in_words)
            return temp; 
        
        temp = temp->next; 
    }

    return NULL; 
}

void printNode(Node* node)
{
    if(node == NULL)
    {
        printf("NULL"); 
        return; 
    }
        

    int start_offset = (int)(node->start - heap); 

    int prevIndex = -1; 
    int nextIndex = -1; 

    if(node->previous != NULL)
        prevIndex = node->previous->allocated_index; 

    if(node->next != NULL)
        nextIndex = node->next->allocated_index; 
    
    printf("{start:%d, size_in_words: %d, allocated_index: %d, is_allocated: %d, prevIndex: %d, nextIndex: %d}",
    start_offset, (int)node->size_in_words, node->allocated_index, node->is_allocated, prevIndex, nextIndex); 
}

void printLinkedList(SortedLinkedList* list)
{
    printf("\nLinkedList: count(total) = %d, count(all.)=%d, count(de-all.) = %d\n", 
        list->count_total, list->count_allocated, list->count_de_allocated);
    Node* temp = list->head; 
    while(temp != NULL)
    {
        printNode(temp); 
        printf(" -> "); 
        temp = temp->next; 
    }
    printf("NULL\n"); 
}

Node* findNodeByPointer(SortedLinkedList* list, void* ptr)
{
    uintptr_t* start = (uintptr_t*)ptr; 
    Node* temp = list->head; 

    while(temp != NULL)
    {
        if(temp->start == start)
            return temp; 
        
        temp = temp->next; 
    }
    return NULL; 
}

/*-------------------------FUNCTIONS/VARIABLES TO MANAGE THE HEAP-------------------------------*/

SortedLinkedList list = 
    {
    .head = NULL, 
    .count_allocated = 0,
    .count_total = 0,
    .count_de_allocated = 0
    }; 
int mem_alloc[MAX_NUMBER_OF_NODES] = {0};
Node node_alloc[MAX_NUMBER_OF_NODES]  = 
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

void initialize(enum LogLevel minimumLogLevel)
{
    int array_size = sizeof(node_alloc)/sizeof(node_alloc[0]); 
    for(int i = 0; i < array_size; i++)
    {
        node_alloc[i].allocated_index = i; 
    }
    list.head = initializeHeadNode(); 
    list.count_total++; 
    list.count_de_allocated++; 
    setMinimumLogLevel(minimumLogLevel); 
}

Node* initializeHeadNode(void)
{
    node_alloc[0].size_in_words = HEAP_CAP_WORDS;
    node_alloc[0].start = heap; 
    mem_alloc[0] = 1; 
    return &node_alloc[0];
}

Node* allocateNode(void)
{
    int array_size = sizeof(node_alloc)/sizeof(node_alloc[0]); 
    for(int i = 0; i < array_size; i++)
    {
        if(mem_alloc[i] == 0) //corresponding node is free
        {
            mem_alloc[i] = 1; //mark node as allocated
            return &node_alloc[i];  
        }
    }
    
    return NULL; 
}

bool deAllocateNode(Node* node)
{
    int array_size = sizeof(node_alloc)/sizeof(node_alloc[0]); 

    if(node->allocated_index < 0 || node->allocated_index >= array_size)
    {
        printf("Index out of bounds... ");
        return false; 
    }

    if(node->is_allocated == false || mem_alloc[node->allocated_index] == 0)
    {
        printf("Invalid selection, node is not allocated. ");
        return false; 
    }

    mem_alloc[node->allocated_index] = 0; 
    node->is_allocated = false; 
    
    list.count_allocated--; 
    list.count_de_allocated++; 

    return true; 
}

void printAllocations(void)
{
    int array_size = sizeof(node_alloc)/sizeof(node_alloc[0]); 
    printf("\nAllocations Array: ");
    for(int i = 0; i < array_size; i++)
    {
        printf("{%d, %d}", i, mem_alloc[i]); 
    }

    printf("\nLinked List Allocations: ");
    for(int i = 0; i < array_size; i++)
    {
        printf("\n%d'th node: ", i); 
        printNode(&node_alloc[i]); 
    }
    printf("\n"); 
}

bool tryDeFragment(Node* prev, Node* current)
{
    //validations
    if(prev == NULL || current == NULL)
    {
        printf("\nCannot merge NULL nodes...\n");
        return false; 
    }

    if(prev->is_allocated == true || current->is_allocated == true)
    {
        printf("\nOne of the nodes is allocated - cannot merge. prev allocated = %d, current allocated = %d\n", 
        prev->is_allocated, current->is_allocated); 
        return false; 
    }

    if(prev->allocated_index != current->previous->allocated_index || 
    current->allocated_index != prev->next->allocated_index)
    {
        printf("\nNodes are not consequental: ");
        printf("\nPrevious: "); 
        printNode(prev); 
        printf("\nCurrent: ");
        printNode(current);
        printf("\n"); 
    }

    printf("Requested to merge nodes %d and %d", prev->allocated_index, current->allocated_index); 
    
    int diff = (int)(current->start - prev->start); 

    if(diff != (int)prev->size_in_words)
    {
        printf("diff is %d, prev size is : %d", diff, (int)prev->size_in_words); 
        return false; 
    }
    
    //we are ready to merge the nodes.

    //1. move node to previous
    prev->next = current->next; 
    prev->size_in_words += current->size_in_words; 
    
    //2. return node to array
    current->next = NULL;
    current->previous = NULL; 
    current->size_in_words = 0; 
    current->start = NULL; 
    mem_alloc[current->allocated_index] = 0; 

    //3. if next node is not NULL -> need to update the previous flag. 
    if(prev->next != NULL)
        prev->next->previous = prev; 

    list.count_total--; 
    list.count_de_allocated--; 

    printf("De fragnment successful"); 

    return true; 
}