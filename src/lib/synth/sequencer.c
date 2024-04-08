#include "sequencer.h"
#include "oscillator.h"
#include "track.h"
#include "tune.h"
#include "utils.h"
#include <stdlib.h>

// How many samples before the end of the note should the envelope start
// releasing. This number is very arbitrary and based on what "sounds good".
const int ENVELOPE_RELEASE_SAMPLES = SAMPLE_RATE / 100;

Envelope *newEnvelope() {
  Envelope *e = malloc(sizeof(Envelope));
  e->state = DONE;
  e->value = 0;
  e->attackPerSample = 0.01;
  e->decayPerSample = 0.001;
  e->sustainLevel = 0.5;
  e->releasePerSample = 0.001;

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

int loadTune(Sequencer *s, Tune *t) {
  if (s->tune)
    return -1;

  s->tune = t;
  // Start currentSample at one else the first note will be skipped because the
  // currentSample is used to determine when to move to the next note.
  // Using zero means we will move to the next note on the first sample (see
  // getNextSampleForChannel)
  s->currentSample = 1;
  s->samplesPerTick = SAMPLE_RATE / (t->bpm * t->ticksPerBeat) * 60;
  s->currentNoteIndex =
      realloc(s->currentNoteIndex, sizeof(int) * t->tracksCount);
  s->oscillators =
      realloc(s->oscillators, sizeof(Oscillator *) * t->tracksCount);
  s->envelopes = realloc(s->envelopes, sizeof(Envelope *) * t->tracksCount);

  if (!s->currentNoteIndex || !s->oscillators || !s->envelopes)
    return -1;

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
  }

  return 0;
}

Sequencer *newSequencer() {
  Sequencer *s = malloc(sizeof(Sequencer));
  s->tune = NULL;
  s->currentSample = 0;
  s->samplesPerTick = 0;
  s->currentNoteIndex = malloc(sizeof(int));
  s->oscillators = malloc(sizeof(Oscillator *));
  s->envelopes = malloc(sizeof(Envelope *));

  return s;
}

int unloadTune(Sequencer *s) {
  freeTune(s->tune);
  s->tune = NULL;
  s->currentSample = s->samplesPerTick = 0;
  return 0;
}

void freeSequencer(Sequencer *s) {
  if (!s)
    return;

  if (s->tune) {
    for (int i = 0; i < s->tune->tracksCount; i++) {
      freeOscillator(s->oscillators[i]);
      s->oscillators[i] = NULL;

      free(s->envelopes[i]);
      s->envelopes[i] = NULL;
    }
    freeTune(s->tune);
    s->tune = NULL;
  }

  free(s->currentNoteIndex);
  free(s->oscillators);
  s->oscillators = NULL;
  free(s->envelopes);
  s->envelopes = NULL;
  free(s);
  s = NULL;
}

float getNextSampleForChannel(Sequencer *s) {
  if (!s->tune)
    return 0;

  int shouldMoveToNextNote = s->currentSample % s->samplesPerTick == 0;
  int shouldStopEnvelope =
      (s->currentSample + ENVELOPE_RELEASE_SAMPLES) % s->samplesPerTick == 0;
  float result = 0;

  // Plays the current sample in every channel
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

      if (isChangingNotes) {
        setNote(oscillator, envelope, new);
      }

      current = new;
    }

    result += oscillate(oscillator) * process(envelope);
  }

  s->currentSample++;

  if (s->currentSample >= s->samplesPerTick) {
    s->currentSample = 0;
  }

  return result / s->tune->tracksCount;
}
