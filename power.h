#ifndef POWER_H
#define POWER_H

#include <dbus/dbus.h>
#include <stdbool.h>

enum POWER_OPTION { HIBERNATE = 0, POWEROFF, REBOOT, SUSPEND };

void spm_exec(enum POWER_OPTION option);
void spm_free(DBusError *error_ptr, DBusConnection *connection, DBusMessage *message);

#endif // POWER_H
