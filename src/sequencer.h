#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "oscillator.h"
#include "track.h"

#define TRACKS 1

typedef struct {
  // The number of the current sample being played
  int currentSample;

  // Keeps track of the current note for each track
  int currentNote[TRACKS];

  // The number of ticks per beat
  int subdivisions;

  // The number of beats per minute
  int bpm;

  // The number of samples per beat
  int samplesPerBeat;

  // The tracks to be played
  Track *tracks[TRACKS];

  // The oscillators for each track
  Oscillator *oscillators[TRACKS];

} Sequencer;

Sequencer *newSequencer(int bpm, int subdivisions);
void setTrack(Sequencer *s, int index, Track *t, Oscillator *o);
float getNextSampleForChannel(Sequencer *s, int channel);

#endif