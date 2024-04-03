#include "note.h"
#include "oscillator.h"
#include <stdlib.h>

bool isSilence(Note symbol) { return symbol.id == -1; }

bool isSameSound(Note a, Note b) { return a.id == b.id; }

Note *newNote(byte note, hex volume, WaveType wave, EffectType effect) {
  static int id = 0;

  Note *s = malloc(sizeof(Note));
  s->pitch = note;
  s->volume = volume;
  s->effect = effect;
  s->wave = wave;
  s->id = id++;
  return s;
}

Note *newRest() {
  Note *s = malloc(sizeof(Note));
  s->pitch = 0;
  s->volume = 0;
  s->effect = NONE;
  s->wave = TRIANGLE;
  s->id = -1;
  return s;
}

void freeSymbol(Note *s) { free(s); }