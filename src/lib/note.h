#ifndef NOTE_H
#define NOTE_H

#include "oscillator.h"
#include "utils.h"

typedef struct {
  byte pitch;
  hex volume;
  WaveType wave;
  EffectType effect;
  long unsigned int id;
} Note;

Note *newNote(byte pitch, hex volume, WaveType wave, EffectType effect);
Note *newRest();
void freeNote(Note *symbol);
bool isSilence(Note symbol);
bool isSameSound(Note a, Note b);

#endif
