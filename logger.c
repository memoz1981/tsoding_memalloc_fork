#include <stdio.h>
#include "logger.h"
#include <string.h>
#include <stdarg.h>

enum LogLevel minimumLogLevel = Debug; 

void setMinimumLogLevel(enum LogLevel logLevel)
{
    minimumLogLevel = logLevel; 
}

void logToConsole(enum LogLevel level, const char *format, ...)
{
    if((int)level < (int)minimumLogLevel)
        return; 
    
    va_list argptr;
    va_start(argptr, format);

    int size = strlen(format) + 20; 
    char formatNew[size];
    memset(formatNew, '\0', size); 
    strcat(formatNew, "\0"); 
    switch(level) {
        case Debug: 
        strcat(formatNew, "Debug: "); 
        break; 

        case Information: 
        strcat(formatNew, "Information: "); 
        break; 

        case Warning: 
        strcat(formatNew, "Warning: "); 
        break; 

        case Error: 
        strcat(formatNew, "Error: "); 
        break; 

        default:
        break; 
    }
    strcat(formatNew, format); 
    strcat(formatNew, "\0"); 

    vfprintf(stdout, formatNew, argptr);
    va_end(argptr);
}