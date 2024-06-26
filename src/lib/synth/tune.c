#include "tune.h"
#include "track.h"
#include <stdlib.h>

void freeTune(Tune *c) {
  for (int i = 0; i < c->tracksCount; i++) {
    freeTrack(c->tracks[i]);
  }
  free(c->tracks);
  c->tracks = NULL;
  free(c);
  c = NULL;
}

Tune *newTune(int tracksCount) {
  Tune *tune = malloc(sizeof(Tune));
  tune->bpm = 10;
  tune->ticksPerBeat = 1;
  tune->beatsCount = 1;
  tune->tracksCount = tracksCount;
  tune->tracks = malloc(sizeof(Track *) * tracksCount);
  return tune;
}
