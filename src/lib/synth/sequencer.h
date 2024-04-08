#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "oscillator.h"
#include "tune.h"

typedef enum { ATTACK, DECAY, SUSTAIN, RELEASE, DONE } EnvelopeState;
extern const int ENVELOPE_RELEASE_SAMPLES;

// Defines a volume envelope. Its value is between 0 and 1 where 0 is
// silence and 1 is full volume associated with the Note being played
typedef struct {
  // How much volume to increase per sample in the attack phase
  float attackPerSample;
  // How much volume to decrease per sample in the decay phase
  float decayPerSample;
  // The level of volume to sustain after the decay phase
  float sustainLevel;
  // How much volume to decrease per sample in the release phase
  float releasePerSample;
  // The current state of the envelope
  EnvelopeState state;
  // The current value of the envelope
  float value;
} Envelope;

// Starts the given envelope
void start(Envelope *e);
// Initiates the release phase of the envelope
void stop(Envelope *e);
// Processes the envelope for the next sample and returns the value of the
// envelope
float process(Envelope *e);

typedef struct {
  // Keeps track of the current sample being played within a single tick
  int currentSample;

  // Keeps track of the index of the current note for each track
  int *currentNoteIndex;

  // How many samples are there in a single tick
  int samplesPerTick;

  // The tune to be played
  Tune *tune;

  // The oscillators for each track
  Oscillator **oscillators;

  // The envelopes for each track.
  // They are not customisable for now, we use them to have smooth transitions
  // across notes
  Envelope **envelopes;
} Sequencer;

// Creates a new sequencer
Sequencer *newSequencer();

// Loads the given tune in the sequencer, allocating all the necessary envelopes
// and oscillators. If a tune has been loaded before, you need to call
// unloadTune first. Returns -1 for failure.
int loadTune(Sequencer *s, Tune *t);

// Unloads the current tune and frees the resources associated with it. It
// doesn't change oscillators and envelopes though, as they will be
// reinitialised in the next loadTune call.
int unloadTune(Sequencer *s);

// Returns the next sample for the given sequencer. It gets the next sample
// from all the tracks, processes the envelopes and returns the average of all
// the samples (i.e., the tracks are mixed equally)
float getNextSampleForChannel(Sequencer *s);

// Frees the memory associated with the given sequencer
void freeSequencer(Sequencer *s);

#endif
