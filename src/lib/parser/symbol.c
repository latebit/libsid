#include "../synth/oscillator.h"
#include "../synth/track.h"
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

Note parseStandardSymbol(char *str) {
  size_t len = strlen(str);

  int invalid = 0;

  invalid +=
      validate(len == SYMBOL_SIZE, "Invalid string size. Got %d, expected %d\n",
               len, SYMBOL_SIZE);

  // Short-circuit to prevent out-of-bounds access
  if (invalid) {
    return newInvalid();
  }

  invalid +=
      validate((str[0] >= 'A' && str[0] <= 'G') || str[0] == NULL_CHAR,
               "Invalid note. Got %c, expected one of A-G or -\n", str[0]);
  invalid += validate(
      str[1] == '#' || str[1] == 'b' || str[1] == NULL_CHAR,
      "Invalid accidental. Got %c, expected one of #, b, or -\n", str[1]);
  invalid +=
      validate((str[2] >= '0' && str[2] <= '7') || str[2] == NULL_CHAR,
               "Invalid octave. Got %c, expected one of 0-7 or -\n", str[2]);
  invalid +=
      validate((str[3] >= '0' && str[3] <= '3') || str[3] == NULL_CHAR,
               "Invalid wave. Got %c, expected one of 0-3 or -\n", str[3]);
  invalid +=
      validate((str[4] >= '0' && str[4] <= 'F') || str[4] == NULL_CHAR,
               "Invalid volume. Got %c, expected one of 0-F or -\n", str[4]);
  invalid +=
      validate((str[5] >= '0' && str[5] <= '4') || str[5] == NULL_CHAR,
               "Invalid effect. Got %c, expected one of 0-4 or -\n", str[5]);

  if (invalid) {
    return newInvalid();
  }

  byte note = 0;
  byte volume = 0;
  EffectType effect = NONE;
  WaveType wave = TRIANGLE;

  if (str[0] == 'C') {
    note = 0;
  } else if (str[0] == 'D') {
    note = 2;
  } else if (str[0] == 'E') {
    note = 4;
  } else if (str[0] == 'F') {
    note = 5;
  } else if (str[0] == 'G') {
    note = 7;
  } else if (str[0] == 'A') {
    note = 9;
  } else if (str[0] == 'B') {
    note = 11;
  } else if (str[0] == NULL_CHAR) {
    return newRest();
  }

  if (str[1] == '#') {
    note++;
  } else if (str[1] == 'b') {
    note--;
  }

  if (str[3] == '0' || str[3] == NULL_CHAR) {
    wave = TRIANGLE;
  } else if (str[3] == '1') {
    wave = SAWTOOTH;
  } else if (str[3] == '2') {
    wave = SQUARE;
  } else if (str[3] == '3') {
    wave = NOISE;
  }

  if (str[5] == '0' || str[5] == NULL_CHAR) {
    effect = NONE;
  } else if (str[5] == '1') {
    effect = DROP;
  } else if (str[5] == '2') {
    effect = SLIDE;
  } else if (str[5] == '3') {
    effect = FADEIN;
  } else if (str[5] == '4') {
    effect = FADEOUT;
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
