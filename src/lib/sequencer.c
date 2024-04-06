#include "sequencer.h"
#include "oscillator.h"
#include "track.h"
#include "tune.h"
#include "utils.h"
#include <stdlib.h>

Envelope *newEnvelope() {
  Envelope *e = malloc(sizeof(Envelope));
  *e = (Envelope){.state = DONE,
                  .value = 0,
                  .attackPerSample = 0.01,
                  .decayPerSample = 0.001,
                  .sustainLevel = 0.5,
                  .releasePerSample = 0.001};
  return e;
}

void start(Envelope *e) { e->state = ATTACK; }

void stop(Envelope *e) { e->state = RELEASE; }

float process(Envelope *e) {
  switch (e->state) {
  case ATTACK:
    e->value += e->attackPerSample;
    if (e->value >= 1) {
      e->value = 1;
      e->state = DECAY;
    }
    break;
  case DECAY:
    e->value -= e->decayPerSample;
    if (e->value <= e->sustainLevel) {
      e->value = e->sustainLevel;
      e->state = SUSTAIN;
    }
    break;
  default:
  case SUSTAIN:
    break;
  case RELEASE:
    e->value -= e->releasePerSample;
    if (e->value <= 0) {
      e->value = 0;
      e->state = DONE;
    }
    break;
  }

  return e->value;
}
void setNote(Oscillator *o, Envelope *e, Note n) {
  setPitch(o, n.pitch);
  setVolume(o, n.volume / 16.0);
  setEffect(o, n.effect);
  setWave(o, n.wave);
  start(e);
}
Sequencer *newSequencer(Tune *t) {
  Sequencer *s = malloc(sizeof(Sequencer));
  s->tune = t;
  s->currentSample = 0;
  s->wrappingFactor = 1;
  s->samplesPerBeat = SAMPLE_RATE / (t->bpm * t->ticksPerBeat) * 60;
  s->currentNoteIndex = malloc(sizeof(int) * t->tracksCount);
  s->oscillators = malloc(sizeof(Oscillator *) * t->tracksCount);
  s->envelopes = malloc(sizeof(Envelope *) * t->tracksCount);

  for (int i = 0; i < t->tracksCount; i++) {
    s->currentNoteIndex[i] = 0;
    s->oscillators[i] = newOscillator(0);
    s->envelopes[i] = newEnvelope();

    // This is used to allow the first note of each track to be executed
    // else the first note will be skipped until there is a change note event
    Note firstNote = get(t->tracks[i], 0);
    if (!isInvalid(firstNote)) {
      setNote(s->oscillators[i], s->envelopes[i], firstNote);
    }

    // Calculate the least common multiple of the wrapping factor so that we can
    // wrap the currentSample counter at the right time
    s->wrappingFactor =
        lcm(s->wrappingFactor, t->tracks[i]->length * s->samplesPerBeat);
  }

  return s;
}

void freeSequencer(Sequencer *s) {
  if (!s)
    return;

  free(s->currentNoteIndex);
  for (int i = 0; i < s->tune->tracksCount; i++) {
    freeOscillator(s->oscillators[i]);
    s->oscillators[i] = NULL;

    free(s->envelopes[i]);
    s->envelopes[i] = NULL;
  }
  free(s->oscillators);
  s->oscillators = NULL;
  free(s->envelopes);
  s->envelopes = NULL;
  freeTune(s->tune);
  s->tune = NULL;
  free(s);
  s = NULL;
}

// How many samples before the end of the note should the envelope start
// releasing
const int ENVELOPE_RELEASE_SAMPLES = SAMPLE_RATE / 100;

float getNextSampleForChannel(Sequencer *s) {
  int shouldMoveToNextNote = s->currentSample % s->samplesPerBeat == 0;
  int shouldStopEnvelope =
      (s->currentSample + ENVELOPE_RELEASE_SAMPLES) % s->samplesPerBeat == 0;
  float result = 0;

  for (int channel = 0; channel < s->tune->tracksCount; channel++) {
    Track *track = s->tune->tracks[channel];
    Envelope *envelope = s->envelopes[channel];
    Oscillator *oscillator = s->oscillators[channel];

    int currentNoteIndex = s->currentNoteIndex[channel];
    int newNoteIndex = (currentNoteIndex + 1) % track->length;

    Note current = get(track, currentNoteIndex);
    Note new = get(track, newNoteIndex);
    bool isChangingNotes = !isSameNote(new, current);

    if (shouldStopEnvelope && isChangingNotes) {
      stop(envelope);
    }

    if (shouldMoveToNextNote) {
      s->currentNoteIndex[channel] = newNoteIndex;

      // Set the frequency and volume of the oscillator
      if (isChangingNotes) {
        setNote(oscillator, envelope, new);
      }

      current = new;
    }

    result += oscillate(oscillator) * process(envelope);
  }

  s->currentSample = (s->currentSample + 1) % (s->wrappingFactor);

  return result / s->tune->tracksCount;
}
