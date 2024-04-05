#include "sequencer.h"
#include "oscillator.h"
#include "track.h"
#include "utils.h"
#include <stdlib.h>

void start(Envelope *e) { e->state = ATTACK; }

void stop(Envelope *e) { e->state = RELEASE; }

void process(Envelope *e) {
  float result = 1;
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
}

Sequencer *newSequencer(int bpm, int subdivisions) {
  Sequencer *s = malloc(sizeof(Sequencer));
  *s = (Sequencer){.currentSample = 0,
                   .bpm = bpm,
                   .subdivisions = subdivisions,
                   .currentNote = {0},
                   .samplesPerBeat = SAMPLE_RATE / (bpm * subdivisions) * 60};

  for (int i = 0; i < TRACKS; i++) {
    s->envelopes[i] = malloc(sizeof(Envelope));
    *s->envelopes[i] = (Envelope){.state = DONE,
                                  .value = 0,
                                  .attackPerSample = 0.01,
                                  .decayPerSample = 0.001,
                                  .sustainLevel = 0.5,
                                  .releasePerSample = 0.001};
  }

  return s;
}

void setTrack(Sequencer *s, int index, Track *t, Oscillator *o) {
  if (index >= TRACKS || index < 0)
    return;
  s->tracks[index] = t;
  s->oscillators[index] = o;
}

void freeSequencer(Sequencer *s) {
  for (int i = 0; i < TRACKS; i++) {
    free(s->oscillators[i]);
    free(s->envelopes[i]);
  }
  free(s);
}

// How many samples before the end of the note should the envelope start
// releasing
const int ENVELOPE_RELEASE_SAMPLES = SAMPLE_RATE / 100;

float getNextSampleForChannel(Sequencer *s) {
  int shouldMoveToNextNote = s->currentSample % s->samplesPerBeat == 0;
  int shouldStopEnvelope =
      (s->currentSample + ENVELOPE_RELEASE_SAMPLES) % s->samplesPerBeat == 0;
  float result = 0;

  for (int channel = 0; channel < TRACKS; channel++) {
    Track *track = s->tracks[channel];
    Envelope *envelope = s->envelopes[channel];
    Oscillator *oscillator = s->oscillators[channel];

    int currentNoteIndex = s->currentNote[channel];
    int newNoteIndex = (currentNoteIndex + 1) % track->length;

    Note current = get(track, currentNoteIndex);
    Note new = get(track, newNoteIndex);

    if (shouldStopEnvelope && !isSameNote(new, current)) {
      stop(envelope);
    }

    if (shouldMoveToNextNote) {
      s->currentNote[channel] = newNoteIndex;

      // Set the frequency and volume of the oscillator
      if (!isSameNote(new, current)) {
        setNote(oscillator, new.pitch);
        setVolume(oscillator, new.volume / 16.0);
        setEffect(oscillator, new.effect);
        setWave(oscillator, new.wave);
        start(envelope);
      }

      current = new;
    }

    process(envelope);
    result += oscillate(oscillator) * envelope->value;
  }

  // TODO: this should be wrapped by the minimum common multiple of the track
  // lengths
  s->currentSample =
      (s->currentSample + 1) % (s->samplesPerBeat * s->tracks[0]->length);

  return result / TRACKS;
}
