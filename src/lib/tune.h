#ifndef TUNE_H
#define TUNE_H

#include "track.h"

typedef struct {
  char *version;
  int bpm;
  int ticksPerBeat;
  int beatsCount;
  int tracksCount;
  int *trackSize;
  Track **tracks;
} Tune;

Tune *newTune(char *version, int bpm, int ticksPerBeat, int beatsCount,
              int tracksCount, int *trackSize, Track **tracks);
void freeTune(Tune *t);

#endif