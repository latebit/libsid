#include "sequencer.h"
#include "symbol.h"
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
    .samplesPerBeat = SAMPLE_RATE / (bpm * subdivisions) * 60
  };
  return s;
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

float getNextSampleForChannel(Sequencer *s) {
  int shouldMoveToNextNote = s->currentSample % s->samplesPerBeat == 0;
  float result = 0;

    for (int channel = 0; channel < TRACKS; channel ++) {
      Symbol current = s->tracks[channel]->notes[s->currentNote[channel]];
      
      if (shouldMoveToNextNote) {
        // Move the note index one step forward
        s->currentNote[channel] = (s->currentNote[channel] + 1) % s->tracks[channel]->length;
        // Retrieve the symbol for the current note
        Symbol new = s->tracks[channel]->notes[s->currentNote[channel]];

        // Set the frequency and volume of the oscillator
        if (!isSameSymbol(new, current)) {
          setNote(s->oscillators[channel], new.note);
          setVolume(s->oscillators[channel], new.volume/16.0);
          setEffect(s->oscillators[channel], new.effect);
          setWave(s->oscillators[channel], new.wave);
        }

        current = new;
      }

      result += oscillate(s->oscillators[channel]);
    }

  // TODO: this should be wrapped by the minimum common multiple of the track lengths
  s->currentSample = (s->currentSample + 1) % (s->samplesPerBeat * s->tracks[0]->length);

  return result / TRACKS;
}