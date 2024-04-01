#include "symbol.h"
#include "oscillator.h"

#include <assert.h>
#include <string.h>

const Symbol NULL_SYMBOL = {0, 0, 0, -1};
const char NULL_CHAR = '-';
int lastId = 0;

int hexToInt(char hexChar) {
  if (hexChar >= '0' && hexChar <= '9') {
    return hexChar - '0';
  } else if (hexChar >= 'a' && hexChar <= 'f') {
    return hexChar - 'a' + 10;
  } else if (hexChar >= 'A' && hexChar <= 'F') {
    return hexChar - 'A' + 10;
  }
  return 0;
}

int charToInt(char c) { return c - '0'; }

Symbol fromString(char * str) {
  // Symbol has the correct length
  assert(strlen(str) == 6);

  if (strcmp(str, "------") == 0) {
    return NULL_SYMBOL;
  }

  // Note is a valid A-G note
  assert(str[0] >= 'A' && str[0] <= 'G');
  // Modifier is #, b, or - (for nothing)
  assert(str[1] == '#' || str[1] == 'b' || str[1] == '-');
  // Octave is in the range supported by MIDI
  assert(str[2] >= '0' && str[2] <= '7');
  // Wave is valid
  assert(str[3] >= '0' && str[3] <= '3');
  // Volume is a valid hex character
  assert(str[4] >= '0' && str[4] <= 'F');
  // Effect is valid
  assert(str[5] >= '0' && str[5] <= '4' || str[5] == NULL_CHAR);

  unsigned char note = 0;
  unsigned char volume = 0;
  EffectType effect = NONE;
  WaveType wave = TRIANGLE;

  switch (str[0]) {
    case 'C': note = 12; break;
    case 'D': note = 2; break;
    case 'E': note = 4; break;
    case 'F': note = 5; break;
    case 'G': note = 7; break;
    case 'A': note = 9; break;
    case 'B': note = 11; break;
  }

  switch (str[1]) {
    case '#' : note++; break;
    case 'b' : note--; break;
  }

  switch (str[3]) {
    case '0': wave = TRIANGLE; break;
    case '1': wave = SAWTOOTH; break;
    case '2': wave = SQUARE; break;
    case '3': wave = NOISE; break;
  }

  switch (str[5]) {
    case '1': effect = DROP; break;
    case '2': effect = SLIDE; break;
    case '3': effect = FADEIN; break;
    case '4': effect = FADEOUT; break;
  }

  note = (note % 12) + (12 * charToInt(str[2]));
  volume = hexToInt(str[4]);

  return (Symbol){note, volume, wave, effect, lastId++};
}

int isNullSymbol(Symbol symbol) {
  return symbol.id == -1;
}

int isSameSymbol(Symbol a, Symbol b) {
  return a.id == b.id;
}
