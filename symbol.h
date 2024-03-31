#ifndef SYMBOL_H
#define SYMBOL_H

typedef struct {
  int type;
  int value;
} Effect;

typedef struct {
  unsigned char note;
  unsigned char volume;
  Effect effect;
} Symbol;

// Creates a Symbol from a tracker entry
Symbol fromString(char *str);

extern const Symbol NULL_SYMBOL;
extern const char NULL_CHAR;
extern const char REPEAT_CHAR;

#endif