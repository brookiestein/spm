#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdbool.h>

void perform(const char* caller_name, const char* message, FILE* stream);
void logger(const char* caller_name, const char* message, FILE* stream);

extern const char* prog_name;
extern char* log_file;
extern bool as_daemon;
extern bool verbose;
extern bool debug;

#endif
