#include "logger.h"

void
logger(const char* caller_name, const void* caller, const char* message, FILE* stream)
{
        fprintf(stream, "%s(%p): %s", caller_name, caller, message);
}
