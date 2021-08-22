#ifndef SPM_H
#define SPM_H

#include <getopt.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
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
        bool monitor;
        bool poweroff;
        bool hibernate;
        bool reboot;
        bool suspend;
        bool help;
        size_t time_to_wait;
} Options;

bool as_daemon;
bool debug;
bool verbose;
char* log_file;
const char* prog_name;
char* locker_cmd;
const char* version = "SPM v1.3";

uint8_t usage(void);
uint8_t exec_option(const Options* options);
void* run_locker(void* locker_cmd);
void run_timer(size_t seconds);
void monitorize(bool monitor);

#endif
