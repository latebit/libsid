#include "error.h"

#include <stdarg.h>
#include <stdio.h>

void verror(const char *message, va_list args) {
  fprintf(stderr, "error: ");
  vfprintf(stderr, message, args);
  fprintf(stderr, "\n");
}

void error(const char *message, ...) {
  va_list args;
  va_start(args, message);
  verror(message, args);
  va_end(args);
}
