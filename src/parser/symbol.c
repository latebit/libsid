#include "../lib/note.h"
#include "../lib/oscillator.h"
#include "error.h"

#include <stdarg.h>
#include <string.h>

const char NULL_CHAR = '-';
const int SYMBOL_SIZE = 6;

const char *REST_SYMBOL = "------";
const char *CONTINUE_SYMBOL = "......";
const char *END_OF_TRACK_SYMBOL = "      ";

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

int digitToInt(char c) { return c - '0'; }

int validate(bool condition, const char *message, ...) {
  if (!condition) {
    va_list args;
    va_start(args, message);
    verror(message, args);
    va_end(args);
    return 1;
  }
  return 0;
}

Note *parseStandardSymbol(char *str) {
  size_t len = strlen(str);

  int invalid = 0;

  invalid +=
      validate(len == SYMBOL_SIZE, "Invalid string size. Got %d, expected %d\n",
               len, SYMBOL_SIZE);

  invalid +=
      validate(str[0] >= 'A' && str[0] <= 'G' || str[0] == NULL_CHAR,
               "Invalid note. Got %c, expected one of A-G or -\n", str[0]);
  invalid += validate(
      str[1] == '#' || str[1] == 'b' || str[1] == NULL_CHAR,
      "Invalid accidental. Got %c, expected one of #, b, or -\n", str[1]);
  invalid +=
      validate(str[2] >= '0' && str[2] <= '7' || str[2] == NULL_CHAR,
               "Invalid octave. Got %c, expected one of 0-7 or -\n", str[2]);
  invalid +=
      validate(str[3] >= '0' && str[3] <= '3' || str[3] == NULL_CHAR,
               "Invalid wave. Got %c, expected one of 0-3 or -\n", str[3]);
  invalid +=
      validate(str[4] >= '0' && str[4] <= 'F' || str[4] == NULL_CHAR,
               "Invalid volume. Got %c, expected one of 0-F or -\n", str[4]);
  invalid +=
      validate(str[5] >= '0' && str[5] <= '4' || str[5] == NULL_CHAR,
               "Invalid effect. Got %c, expected one of 0-4 or -\n", str[5]);

  if (invalid) {
    return NULL;
  }

  byte note = 0;
  byte volume = 0;
  EffectType effect = NONE;
  WaveType wave = TRIANGLE;

  switch (str[0]) {
  case 'C':
    note = 0;
    break;
  case 'D':
    note = 2;
    break;
  case 'E':
    note = 4;
    break;
  case 'F':
    note = 5;
    break;
  case 'G':
    note = 7;
    break;
  case 'A':
    note = 9;
    break;
  case 'B':
    note = 11;
    break;
  case NULL_CHAR:
    return newRest();
  }

  switch (str[1]) {
  case '#':
    note++;
    break;
  case 'b':
    note--;
    break;
  }

  switch (str[3]) {
  case '0':
  case NULL_CHAR:
    wave = TRIANGLE;
    break;
  case '1':
    wave = SAWTOOTH;
    break;
  case '2':
    wave = SQUARE;
    break;
  case '3':
    wave = NOISE;
    break;
  }

  switch (str[5]) {
  case '0':
  case NULL_CHAR:
    effect = NONE;
    break;
  case '1':
    effect = DROP;
    break;
  case '2':
    effect = SLIDE;
    break;
  case '3':
    effect = FADEIN;
    break;
  case '4':
    effect = FADEOUT;
    break;
  }

  int octave = str[2] == NULL_CHAR ? 4 : digitToInt(str[2]);

  /**
  The octave number is tied to the alphabetic character used to describe the
  pitch, with the division between note letters ‘B’ and ‘C’, thus:

  * "B3" and "B#3" are designated as being in octave "3"
  * "C4" and "Cb4" would properly be designated in octave "4"

  even though the pitch is the same.
  */
  if (note < 0) {
    note = 12 + note;
    octave--;
  } else if (note > 11) {
    note = note - 12;
    octave++;
  }
  note += (12 * octave);

  volume = str[4] == NULL_CHAR ? 8 : hexToInt(str[4]);

  return newNote(note, volume, wave, effect);
}

bool isRestSymbolString(char *str) {
  return str != NULL && strcmp(str, REST_SYMBOL) == 0;
}

bool isContinueSymbolString(char *str) {
  return str != NULL && strcmp(str, CONTINUE_SYMBOL) == 0;
}

bool isEndOfTrackSymbolString(char *str) {
  return str == NULL || strcmp(str, END_OF_TRACK_SYMBOL) == 0;
}