#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

void logger(const char* caller_name, const void* caller, const char* message, FILE* stream);

extern char* log_file;

#endif
