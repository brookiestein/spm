#include "power_options.h"

static void
spm_free(DBusError* error, DBusConnection* connection, DBusMessage* message,
        const char* message_to_log)
{
        uint16_t size = strlen(message_to_log) + 3;
        if (error->message)
                size += strlen(error->message);
        char* msg;

        if (strncmp(message_to_log, "Cleaning", 8) == 0)
                msg = format(strlen(message_to_log) + 1, "%s\n", message_to_log);
        else
                msg = format(size + 1, "%s: %s\n", message_to_log, error->message);

        logger("spm_free", msg, stderr);
        free(msg);

        if (error != NULL)
                dbus_error_free(error);
        if (connection != NULL)
                dbus_connection_unref(connection);
        if (message != NULL)
                dbus_message_unref(message);
}

void*
lock_as_needed(void* data)
{
        const char* method = (char*) data;
        if (strncmp(method, SUSPEND, strlen(SUSPEND)) == 0
                || strncmp(method, HIBERNATE, strlen(HIBERNATE)) == 0)
        {
                run_locker(locker_cmd);
        }
        return data;
}

uint8_t
spm_power(const char* method)
{
        DBusConnection* connection;
        DBusMessage* message;
        DBusError error;
        DBusBusType bus_type = DBUS_BUS_SYSTEM;
        int timeout = DBUS_TIMEOUT_USE_DEFAULT;

        dbus_error_init(&error);
        connection = dbus_bus_get(bus_type, &error);
        if (connection == NULL) {
                spm_free(&error, NULL, NULL, "There was an error while making the connection");
                return 1;
        }

        message = dbus_message_new_method_call(DEST, PATH, NAME, method);
        if (message == NULL) {
                spm_free(&error, connection, NULL, "There was an error while making the message to the bus");
                return 1;
        }

        dbus_message_set_auto_start(message, FALSE);

        if (dbus_message_append_args(message, DBUS_TYPE_BOOLEAN, EXTRA, DBUS_TYPE_INVALID) != TRUE) {
                spm_free(&error, connection, message, "There was an error while appending args to the message");
                return 1;
        }

        /* If user wants to lock their screen before suspend or hibernate... */
        pthread_t lock_id;
        /* I know what I'm doing is horrible (casting a const pointer to a non-const one),
         * but the lock_as_needed function although it receives it as a non-const value,
         * it treats it as a const one.
         * It's done in this way because of pthread_create requirements. */
        pthread_create(&lock_id, NULL, lock_as_needed, (void *) method);

        dbus_connection_send_with_reply_and_block(connection, message, timeout, &error);

        if (dbus_error_is_set(&error)) {
                spm_free(&error, connection, message, "There was an error");
                return 1;
        }

        char* message_to_log = format(strlen(method) + 5, "%s-ing\n", method);
        logger("spm_power", message_to_log, stdout);
        free(message_to_log);

        spm_free(&error, connection, message, "Cleaning");
        pthread_join(lock_id, NULL);
        return 0;
}
