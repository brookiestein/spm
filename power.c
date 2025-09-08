#include "power.h"
#include "util.h"

#include <dbus/dbus.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define DEST "org.freedesktop.login1"
#define PATH "/org/freedesktop/login1"
#define NAME "org.freedesktop.login1.Manager"
#define EXTRA "boolean:true"

void
spm_exec(enum POWER_OPTION option)
{
	 const char *METHOD = get_method(option);
	 
	 DBusError error;
	 dbus_error_init(&error);

	 DBusConnection *connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	 if (!connection)
		  die("An error occurred while getting the system bus.");

	 DBusMessage *message = dbus_message_new_method_call(DEST, PATH, NAME, METHOD);

	 dbus_message_set_auto_start(message, FALSE);

	 if (!dbus_message_append_args(message, DBUS_TYPE_BOOLEAN, EXTRA, DBUS_TYPE_INVALID)) {
		  spm_free(&error, connection, message);
		  die("An error occurred while setting DBus up.");
	 }

	 dbus_connection_send_with_reply_and_block(connection, message, DBUS_TIMEOUT_USE_DEFAULT, &error);
	 if (dbus_error_is_set(&error)) {
		  spm_free(&error, connection, message);
		  die("An error occurred while sending the DBus message: '%s'.", METHOD);
	 }

	 spm_free(&error, connection, message);
}

void
spm_free(DBusError *error_ptr, DBusConnection *connection, DBusMessage *message)
{
	 if (error_ptr)
		  dbus_error_free(error_ptr);
	 if (connection)
		  dbus_connection_unref(connection);
	 if (message)
		  dbus_message_unref(message);
}
