#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "format.h"
#include "logger.h"
#include "power_options.h"
#include "get_time.h"

#define CAPACITY_FILE "/sys/class/power_supply/BAT0/capacity"
#define STATUS_FILE "/sys/class/power_supply/BAT0/status"
#define CH "Charging"
#define DISCH "Discharging"
#define CHARGE_WARNING 25
#define CHARGE_LIMIT 15
#define MAX_WARNS 3
#define TIME 20 /* Verify the battery's charge every TIME seconds. */

void* battery_monitor(void* data);

#endif
