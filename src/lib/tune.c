#include "tune.h"
#include "track.h"
#include <stdlib.h>

void freeTune(Tune *c) {
  for (int i = 0; i < c->tracksCount; i++) {
    freeTrack(c->tracks[i]);
  }
  free(c->tracks);
}

Tune *newTune(int tracksCount) {
  Tune *tune = malloc(sizeof(Tune));
  tune->bpm = 0;
  tune->ticksPerBeat = 0;
  tune->beatsCount = 0;
  tune->tracksCount = tracksCount;
  tune->tracks = malloc(sizeof(Track *) * tracksCount);
  return tune;
}