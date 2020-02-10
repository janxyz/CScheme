#include "error.h"
// va_list
#include <stdarg.h>
// stderr
#include <stdio.h>
// exit, EXIT_FAILURE
#include <stdlib.h>

void exit_with_error(char const* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(EXIT_FAILURE);
}
