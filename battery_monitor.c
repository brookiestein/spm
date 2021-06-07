#include "battery_monitor.h"

void*
battery_monitor(void* data)
{
        (void) data; /* For now this won't be used. */
        short warnings = 0;
        char* time = get_time();
        char* fmt = "[%s]: Battery monitor started.\n";
        size_t size = strlen(fmt) + strlen(time);
        char* message = format(size, fmt, time);

        free(time);
        logger("battery_monitor", &battery_monitor, message, stdout);
        free(message);

        while (true) {
                FILE* capacity_file = fopen(CAPACITY_FILE, "r");
                FILE* status_file = fopen(STATUS_FILE, "r");
                size = 20; /* Enough space for files' content. */
                char* percentage = (char*) malloc(size * sizeof(char));
                char* status = (char*) malloc(size * sizeof(char));

                percentage = fgets(percentage, size, capacity_file);
                status = fgets(status, size, status_file);
                short percent = atoi(percentage);

                free(percentage);
                fclose(capacity_file);
                fclose(status_file);

                status[strlen(status) - 1] = '\0'; /* Remove the '\n' character. */

                bool suspend = false;
                bool log = false;
                fmt = "[%s]: %s, actual battery level: %i%%\n";
                time = get_time();
                char* real_status = strncmp(status, "Unknown", 7) == 0 ? "Fully charged" : status;
                size = strlen(fmt) + strlen(time) + strlen(real_status);
                message = format(size, fmt, time, real_status, percent);
                logger("battery_monitor", &battery_monitor, message, stdout);
                free(time);
                free(message);

                if (percent <= CHARGE_WARNING && warnings < MAX_WARNS
                        && strncmp(status, DISCH, strlen(DISCH)) == 0)
                {
                        ++warnings;
                        time = get_time();
                        fmt = "[%s]: Battery's level is arriving to its limit.";
                        size = strlen(fmt) + strlen(time);
                        message = format(size, fmt, time);
                        log = true;
                }
                else if (percent <= CHARGE_LIMIT
                        && strncmp(status, DISCH, strlen(DISCH)) == 0)
                {
                        time = get_time();
                        fmt = "[%s]: Battery's level is too low. Going to sleep.";
                        size = strlen(fmt) + strlen(time);
                        message = format(size, fmt, time);
                        suspend = true;
                        log = true;
                }

                if (log)
                {
                        logger("battery_monitor", &battery_monitor, message, stdout);
                        free(time);
                        free(message);
                }

                free(status);

                if (suspend)
                        spm_power(SUSPEND);

                sleep(TIME);
        }
}
