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
  long unsigned int id;
} Symbol;

// Creates a Symbol from a tracker entry
Symbol fromString(char *str);

int isNullSymbol(Symbol symbol);
int isSameSymbol(Symbol a, Symbol b);

extern const Symbol NULL_SYMBOL;
extern const char NULL_CHAR;

#endif
