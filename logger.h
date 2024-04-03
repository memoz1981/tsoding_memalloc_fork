#ifndef LOGGER_H_
#define LOGGER_H_

enum LogLevel { Debug, Information, Warning, Error}; 

void log_level(char* message, enum LogLevel level);

#endif
