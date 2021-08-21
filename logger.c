#include "logger.h"

void
logger(const char* caller_name, const void* caller, const char* message, FILE* stream)
{
        if (log_file) {
                FILE* filelog = fopen(log_file, "r");

                if (filelog) {
                        fclose(filelog);
                        filelog = fopen(log_file, "a");
                } else {
                        filelog = fopen(log_file, "w");
                }

                fprintf(filelog, "%s: %s", caller_name, message);
                fclose(filelog);
        }

        if (caller) {
                fprintf(stream, "%s(%p): %s", caller_name, caller, message);
        } else {
                fprintf(stream, "%s: %s", caller_name, message);
        }
}
