#ifndef PARSER_H
#define PARSER_H

#include "../synth/tune.h"

Tune *fromFile(char *filename);
Tune *fromString(char *str);

#endif
