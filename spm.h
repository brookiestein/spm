#ifndef SPM_H
#define SPM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "format.h"
#include "logger.h"
#include "power_options.h"
#include "battery_monitor.h"

#ifdef _ENABLE_GUI
#include "gui.h"
#endif /* ENABLE_GUI */

typedef struct
{
        bool daemon;
        bool monitor;
        bool poweroff;
        bool hibernate;
        bool reboot;
        bool suspend;
        bool help;
} Options;

uint8_t usage(void);
char** make_lowercase(const int* argc, char** argv);
char* spm_strncpy(char* dest, const char* src, const size_t* start, const size_t* end);
/* This function will delete the -, and -- from command line arguments if needed. */
char** make_easier_to_work(const int* argc, char** argv);
Options* look_for_an_option(const int* argc, char** argv, const char* large_options[],
                const char* short_options, const uint8_t* options_size, char** unrecognized,
                size_t* size, Options* parallel_options);
uint8_t exec_option(const Options* options);
bool confirm(void);

#endif
