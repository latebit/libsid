#ifndef SYMBOL_H
#define SYMBOL_H

#include "oscillator.h"
#include "utils.h"

typedef struct {
  byte note;
  hex volume;
  WaveType wave;
  EffectType effect;
  long unsigned int id;
} Symbol;

// Creates a Symbol from a tracker entry
Symbol fromString(char *str);

int isNullSymbol(Symbol symbol);
int isSameSymbol(Symbol a, Symbol b);

extern const Symbol NULL_SYMBOL;
extern const char NULL_CHAR;

#endif
