#include "common.h"

void fatal_error(const char* fmt, ...)
{
    int errno_save = errno;
    va_list args; 

    // Extracts the arguments into the va_list
    va_start(args, fmt);

    // Somewhat similar to fprintf, but accepts the
    // additional arguments as a va_list instead of 
    // directly writing them in the function call
    vfprintf(stdout, fmt, args);

    printf("\n");
    fflush(stdout);

    // print out error messages if errno was set
    if (errno_save != 0) {
        printf("(errno = %d) : %s\n", errno_save, strerror(errno_save));
        fflush(stdout);
    }
    va_end(args);

    exit(1);
}
