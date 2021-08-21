#include "logger.h"

void
logger(const char* caller_name, const char* message, FILE* stream)
{
        if (log_file) {
                FILE* filelog = fopen(log_file, "r");

                if (filelog) {
                        fclose(filelog);
                        filelog = fopen(log_file, "a");
                } else {
                        filelog = fopen(log_file, "w");
                }

                perform(caller_name, message, filelog);
                fclose(filelog);
        }

        if (verbose && !as_daemon)
                perform(caller_name, message, stream);
}

void
perform(const char* caller_name, const char* message, FILE* stream)
{
        fprintf(stream, "%s: %s", (debug ? caller_name : prog_name), message);
}
