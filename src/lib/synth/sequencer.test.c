#include "sequencer.h"
#include "../../../test/test.h"
#include "track.h"
#include "tune.h"

void envelope() {
  Envelope e = {1, 1, 1, 1};

  start(&e);
  expectEqlInt(e.value, 0, "starts at 0");
  expectEqlInt(e.state, ATTACK, "starts in attack");

  process(&e);
  expectEqlInt(e.value, 1, "attack completes");
  expectEqlInt(e.state, DECAY, "moves to decay");
  process(&e);
  expectEqlInt(e.state, SUSTAIN, "moves to sustain");
  expectEqlInt(e.value, e.sustainLevel, "sustain level reached");

  stop(&e);
  expectEqlInt(e.state, RELEASE, "moves to release");
  process(&e);
  expectEqlInt(e.value, 0, "returns to 0");
  expectEqlInt(e.state, DONE, "done");
}

void init() {
  Tune *t = newTune(3);
  t->tracks[0] = newTrack(1);
  push(t->tracks[0], newNote(48, 8, 0, 0));
  t->tracks[1] = newTrack(2);
  push(t->tracks[1], newNote(48, 8, 0, 0));
  push(t->tracks[1], newNote(48, 8, 0, 0));
  t->tracks[2] = newTrack(3);
  push(t->tracks[2], newNote(48, 8, 0, 0));
  push(t->tracks[2], newNote(48, 8, 0, 0));
  push(t->tracks[2], newNote(48, 8, 0, 0));

  Sequencer *s = newSequencer(t);

  expectEqlInt(s->tune->tracksCount, 3, "has correct tracks count");
  expectEqlInt(s->currentSample, 1, "starts at sample 1");
  expectEqlInt(s->samplesPerTick, 264600, "sets correct samples per beat");

  for (int i = 0; i < s->tune->tracksCount; i++) {
    expectEqlInt(s->currentNoteIndex[i], 0, "starts at note 0");
    expectNotNull(s->oscillators[i], "has oscillator");
    expectNotNull(s->envelopes[i], "has enevlope");
  }
  freeSequencer(s);
}

void run() {
  Tune *t = newTune(3);
  t->tracks[0] = newTrack(2);
  push(t->tracks[0], newNote(48, 8, 0, 0));
  push(t->tracks[0], newNote(49, 8, 0, 0));
  t->tracks[1] = newTrack(2);
  push(t->tracks[1], newNote(48, 8, 0, 0));
  push(t->tracks[1], newNote(49, 8, 0, 0));
  t->tracks[2] = newTrack(3);
  push(t->tracks[2], newNote(48, 8, 0, 0));
  push(t->tracks[2], newNote(49, 8, 0, 0));
  push(t->tracks[2], newNote(50, 8, 0, 0));
  t->beatsCount = 2;

  Sequencer *s = newSequencer(t);

  getNextSampleForChannel(s);

  expectEqlInt(s->currentSample, 2, "advances sample");
  expectEqlInt(s->currentNoteIndex[0], 0, "does not advance note (0)");
  expectEqlInt(s->currentNoteIndex[1], 0, "does not advance note (1)");
  expectEqlInt(s->currentNoteIndex[2], 0, "does not advance note (2)");

  // advance until the envelope starts closing
  for (int i = 2; i < s->samplesPerTick - ENVELOPE_RELEASE_SAMPLES; i++) {
    getNextSampleForChannel(s);
  }

  // close the envelope
  getNextSampleForChannel(s);
  expectEqlInt(s->envelopes[0]->state, RELEASE, "releases envelope (0)");
  expectEqlInt(s->envelopes[1]->state, RELEASE, "releases envelope (1)");
  expectEqlInt(s->envelopes[2]->state, RELEASE, "releases envelope (2)");

  // advacne until the end of the tick
  for (int i = s->samplesPerTick - ENVELOPE_RELEASE_SAMPLES + 1;
       i < s->samplesPerTick; i++) {
    getNextSampleForChannel(s);
  }

  // start the next note
  getNextSampleForChannel(s);
  for (int i = 0; i < s->tune->tracksCount; i++) {
    expectEqlInt(s->currentNoteIndex[i], 1, "advances notes");
    expectEqlInt(s->envelopes[i]->state, ATTACK, "resets envelope");
  }

  freeSequencer(s);
}

int main(void) {
  test(envelope);
  test(init);
  test(run);
  return report();
}
