#include <stdio.h>
#include "logger.h"
#include <string.h>

void log_level(char* message, enum LogLevel level)
{
    int length = strlen(message) + 20;
    char* output = (char*)malloc(length * sizeof(char));
    memset(output, 0, length); 

    switch(level) {
        case Debug: 
        strcat(output, "Debug: "); 
        break; 

        case Information: 
        strcat(output, "Information: "); 
        break; 

        case Warning: 
        strcat(output, "Warning: "); 
        break; 

        case Error: 
        strcat(output, "Error: "); 
        break; 

        default:
        break; 
    }

    strcat(output, message); 

    puts(output); 
}