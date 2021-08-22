#ifndef POWER_OPTIONS_H
#define POWER_OPTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <dbus/dbus.h>

#include "logger.h"
#include "format.h"
#include "run_locker.h"

#define DEST "org.freedesktop.login1"
#define PATH "/org/freedesktop/login1"
#define NAME "org.freedesktop.login1.Manager"
#define EXTRA "boolean:true"
#define POWEROFF "PowerOff"
#define HIBERNATE "Hibernate"
#define REBOOT "Reboot"
#define SUSPEND "Suspend"

extern char* locker_cmd;

uint8_t spm_power(const char* method);

#endif
