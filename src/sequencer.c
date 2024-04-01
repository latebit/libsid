#include "sequencer.h"
#include "oscillator.h"
#include "symbol.h"
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
    freeTrack(s->tracks[i]);
    free(s->oscillators[i]);
    free(s->envelopes[i]);
  }
  free(s);
}

float getNextSampleForChannel(Sequencer *s) {
  int shouldMoveToNextNote = s->currentSample % s->samplesPerBeat == 0;
  int shouldStopEnvelope = (s->currentSample + 441) % s->samplesPerBeat == 0;
  float result = 0;

  for (int channel = 0; channel < TRACKS; channel++) {
    int newNoteIndex =
        (s->currentNote[channel] + 1) % s->tracks[channel]->length;
    Symbol current = s->tracks[channel]->notes[s->currentNote[channel]];
    Symbol new = s->tracks[channel]->notes[newNoteIndex];

    if (shouldStopEnvelope && !isSameSymbol(new, current)) {
      stop(s->envelopes[channel]);
    }

    if (shouldMoveToNextNote) {
      s->currentNote[channel] = newNoteIndex;

      // Set the frequency and volume of the oscillator
      if (!isSameSymbol(new, current)) {
        setNote(s->oscillators[channel], new.note);
        setVolume(s->oscillators[channel], new.volume / 16.0);
        setEffect(s->oscillators[channel], new.effect);
        setWave(s->oscillators[channel], new.wave);
        start(s->envelopes[channel]);
      }

      current = new;
    }

    process(s->envelopes[channel]);
    result += oscillate(s->oscillators[channel]) * s->envelopes[channel]->value;
  }

  // TODO: this should be wrapped by the minimum common multiple of the track
  // lengths
  s->currentSample =
      (s->currentSample + 1) % (s->samplesPerBeat * s->tracks[0]->length);

  return result / TRACKS;
}
