#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

#include "power_options.h"

uint8_t gui(int* argc, char **argv);

#endif
