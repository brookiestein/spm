#ifndef UTIL_H
#define UTIL_H

#include <stdarg.h>

#include "power.h"

void die(const char *fmt, ...);
const char *get_method(enum POWER_OPTION option);
const char *get_gerund(enum POWER_OPTION option);

#endif // UTIL_H
