#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void validate(bool condition, const char *message, ...) {
  if (!condition) {
    va_list args;
    va_start(args, message);
    fprintf(stderr, "error: ");
    vfprintf(stderr, message, args);
    va_end(args);
    exit(1);
  }
}
