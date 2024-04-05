#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "oscillator.h"
#include "track.h"

#define TRACKS 3

typedef enum { ATTACK, DECAY, SUSTAIN, RELEASE, DONE } EnvelopeState;

typedef struct {
  float attackPerSample;
  float decayPerSample;
  float sustainLevel;
  float releasePerSample;
  EnvelopeState state;
  float value;
} Envelope;

void start(Envelope *e);
void stop(Envelope *e);
void process(Envelope *e);

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

  // Used to wrap the current sample back to 0 so that we can loop the track
  // indefinitely
  int wrappingFactor;

  // The tracks to be played, they are owned by the Tune
  Track *tracks[TRACKS];

  // The oscillators for each track
  Oscillator *oscillators[TRACKS];

  // The envelopes for each track.
  // They are not customisable for now, we use them to have smooth transitions
  // across notes
  Envelope *envelopes[TRACKS];
} Sequencer;

Sequencer *newSequencer(int bpm, int subdivisions);
void setTrack(Sequencer *s, int index, Track *t, Oscillator *o);
float getNextSampleForChannel(Sequencer *s);
void freeSequencer(Sequencer *s);

#endif
