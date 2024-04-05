#ifndef TUNE_H
#define TUNE_H

#include "track.h"

// The tune is the main structure that represents a song/composition/sound
// effect. It contains the tempo, the number of beats, the number of tracks and
// the tracks themselves.
//
// See the docs/spec-v0.md for more information.
typedef struct {
  // The number of beats per minute
  int bpm;
  // The smallest number of notes that can be played per beat
  int ticksPerBeat;
  // How many beats in the tune
  int beatsCount;
  // The number of tracks in the tune
  int tracksCount;
  // The tracks in the tune
  Track **tracks;
} Tune;

// Allocates a tune setting the number of tracks
Tune *newTune(int tracksCount);
// Frees the tune
void freeTune(Tune *t);

#endif