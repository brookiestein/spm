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
        bool daemon;
        bool monitor;
        bool poweroff;
        bool hibernate;
        bool reboot;
        bool suspend;
        bool help;
} Options;

uint8_t usage(void);
uint8_t exec_option(const Options* options);

#endif
