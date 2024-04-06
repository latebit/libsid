#ifndef PARSER_ERROR_H
#define PARSER_ERROR_H

#include <stdio.h>
void error(const char *message, ...);
void verror(const char *message, va_list args);

#endif
