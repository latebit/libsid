#ifndef TRACK_H
#define TRACK_H

#include "oscillator.h"
#include "utils.h"

typedef struct {
  byte pitch;
  hex volume;
  WaveType wave;
  EffectType effect;
  long unsigned int id;
} Note;

typedef struct {
  Note *notes;
  byte length;
  byte capacity;
} Track;

Track *newTrack(byte length);
void freeTrack(Track *track);
int push(Track *track, Note note);
Note get(Track *track, byte index);

Note newNote(byte pitch, hex volume, WaveType wave, EffectType effect);
Note newRest();
Note newInvalid();
bool isRest(Note symbol);
bool isSameNote(Note a, Note b);
bool isEqualNote(Note a, Note b);
bool isInvalid(Note symbol);

#endif
