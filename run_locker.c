#include "run_locker.h"

void*
run_locker(void* path)
{
        if (!path)
                return NULL;

        FILE* fp = popen((char*) path, "r");
        pclose(fp);
        return path;
}
