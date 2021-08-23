#include "run_locker.h"

void*
run_locker(void* path)
{
        if (!path) {
#ifdef _DEFAULT_LOCKER
                path = _DEFAULT_LOCKER;
#else
                return NULL;
#endif
        }

        FILE* fp = popen((char*) path, "r");
        pclose(fp);
        return path;
}
