#ifndef TUNE_H
#define TUNE_H

#include "track.h"

typedef struct {
  int bpm;
  int ticksPerBeat;
  int beatsCount;
  int tracksCount;
  int *trackSize;
  Track **tracks;
} Tune;

Tune *newTune(int tracksCount);
void freeTune(Tune *t);

#endif