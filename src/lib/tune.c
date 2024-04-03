#include "tune.h"
#include "track.h"
#include <stdlib.h>

void freeTune(Tune *c) {
  for (int i = 0; i < c->tracksCount; i++) {
    freeTrack(c->tracks[i]);
  }
  free(c->tracks);
}

Tune *newTune(char *version, int bpm, int ticksPerBeat, int beatsCount,
              int tracksCount, int *trackSize, Track **tracks) {
  Tune *tune = malloc(sizeof(Tune));
  tune->version = version;
  tune->bpm = bpm;
  tune->ticksPerBeat = ticksPerBeat;
  tune->beatsCount = beatsCount;
  tune->tracksCount = tracksCount;
  tune->tracks = malloc(sizeof(Track *) * tracksCount);
  return tune;
}