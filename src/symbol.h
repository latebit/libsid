#ifndef SYMBOL_H
#define SYMBOL_H

#include "utils.h"

typedef struct {
  int type;
  int value;
} Effect;

typedef struct {
  byte note;
  byte volume;
  Effect effect;
} Symbol;

// Creates a Symbol from a tracker entry
Symbol fromString(char *str);

extern const Symbol NULL_SYMBOL;
extern const char NULL_CHAR;

#endif