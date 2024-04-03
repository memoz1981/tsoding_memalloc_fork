#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "./heap.h"
#include "./linked_list.h"

int run_cycle(void); 

int main()
{
    InitializeAllocators(); 

    int result = 0; 

    while(result != -1)
    {
        result = run_cycle(); 
        print_linked_list(&list); 
    }
    
}

int run_cycle(void)
{
    printf("\n\n\nChoose one of the options below: \n");
    printf("[c] to clear the screen\n");
    printf("[a]_[size_in_bytes] to allocate...(a 10 will allocate 10 bytes)\n"); 
    printf("[d]_[index] to de-allocate...(d 0 will de-allocate 0th index)\n"); 
    printf("[e] to exit\n");

    char command = '\0';
    scanf("%c", &command); 

    if(command == 'e')
        return -1; 
    
    if(command == 'c')
        system("clear"); 
    
    if(command == 'a')
    {
        int num; 
        scanf("Enter the size in bytes: %d", &num); 

        void * pointer = heap_alloc(num);
        if(pointer == NULL)
        {
            printf("Memory could not be allocated...\n");
            return -1; 
        } 

        printf("Memory with address %p was allocated.", pointer); 
        return 0; 
    }

    
    return 0; 
}
