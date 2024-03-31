#include "sequencer.h"
#include "track.h"
#include "oscillator.h"
#include "utils.h"
#include <stdlib.h>

Sequencer* newSequencer(int bpm, int subdivisions) {
  Sequencer *s = malloc(sizeof(Sequencer));
  *s = (Sequencer) {
    .currentSample = 0,
    .bpm = bpm,
    .subdivisions = subdivisions,
    .currentNote = {0},
    .samplesPerBeat = SAMPLE_RATE / (bpm * subdivisions) * 60,
  };
  return s;;
}

void setTrack(Sequencer *s, int index, Track *t, Oscillator *o) {
  if (index >= TRACKS || index < 0) return;
  s->tracks[index] = t;
  s->oscillators[index] = o;
}

void freeSequencer(Sequencer *s) {
  for (int i = 0; i < TRACKS; i++) {
    freeTrack(s->tracks[i]);
    free(s->oscillators[i]);
  }
  free(s);
}

float getNextSampleForChannel(Sequencer *s, int channel) {
  int shouldMoveToNextNote = s->currentSample % s->samplesPerBeat == 0;

  if (shouldMoveToNextNote) {
      s->currentNote[channel] = (s->currentNote[channel] + 1) % s->tracks[channel]->length;
      Symbol sym = s->tracks[channel]->notes[s->currentNote[channel]];
      setFrequency(s->oscillators[channel], NOTE_TO_FREQUENCY[sym.note]);
  }

  // TODO: this should be wrapped by the minimum common multiple of the track lengths
  s->currentSample = (s->currentSample + 1) % (s->samplesPerBeat * s->tracks[0]->length);

  return getNextSample(s->oscillators[channel]);
}