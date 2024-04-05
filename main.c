#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "./heap.h"
#include "./linked_list.h"

int run_cycle(void); 
int read_integer(char* command);

int main()
{
    InitializeAllocators(); 

    int result = 0; 

    while(result != -1)
    {
        result = run_cycle(); 
        printf("Printing linked list: \n"); 
        print_linked_list(&list); 
        printf("Printing allocations: \n"); 
        
        print_allocations(); 
    }
    
}

int run_cycle(void)
{
    printf("\n\n\nChoose one of the options below: \n");
    printf("[c] to clear the screen\n");
    printf("[a]_[size_in_bytes] to allocate...(a_10 will allocate 10 bytes)\n"); 
    printf("[d]_[index] to de-allocate...(d_0 will de-allocate 0th index)\n"); 
    printf("[e] to exit\n\n\n");

    char command[20];
    memset(command, '\0', 20); 
    scanf("%s", command); 
    
    if(strcmp("e", command) == 0)
        return -1; 
    else if(strcmp("c", command) == 0)
        system("clear"); 
    else if(command[0] == 'a')
    {
        system("clear"); 
        int num = read_integer(command); 

        printf("Requested to allocate memory of %d bytes \n", num); 

        void * pointer = heap_alloc(num);
        if(pointer == NULL)
        {
            printf("Memory could not be allocated...\n");
            return 0; 
        } 

        return 0; 
    }
    else if(command[0] == 'd')
    {
        system("clear"); 
        int num = read_integer(command); 

        printf("Requested to free node with allocation index %d \n", num); 

        heap_free((void*)node_alloc[num].start);

        return 0; 
    }
    else
    {
        printf("Entered command %s is invalid", command); 
    }

    return 0; 
}

int read_integer(char* command)
{
    int result = 0; 

    for(int i =0; i<= (int)strlen(command); i++)
    {
        if(command[i] >=48 && command[i] <= 57)
            result = result *10 + command[i] - 48; 
    }

    return result; 
}
