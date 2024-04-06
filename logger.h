#ifndef LOGGER_H_
#define LOGGER_H_
#include <stdarg.h>

enum LogLevel { Debug, Information, Warning, Error}; 

void setMinimumLogLevel(enum LogLevel logLevel); 

void logToConsole(enum LogLevel level, const char *format, ...); 

#endif
