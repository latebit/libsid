#include "track.h"

#include <stdlib.h>
#include <string.h>

Track *newTrack(byte length) {
  Track *track = malloc(sizeof(Track));
  track->notes = malloc(sizeof(Note) * length);
  track->capacity = length;
  track->length = 0;
  return track;
}

void freeTrack(Track *track) {
  free(track->notes);
  track->notes = NULL;
  track->length = track->capacity = 0;
  free(track);
}

int push(Track *track, Note note) {
  if (track->length >= track->capacity) {
    return -1;
  }
  track->notes[track->length++] = note;
  return track->length;
}

Note get(Track *track, byte index) {
  if (index >= track->length || index < 0) {
    return newInvalid();
  }
  return track->notes[index];
}

bool isRest(Note symbol) { return symbol.id == -1; }
bool isSameNote(Note a, Note b) { return a.id == b.id; }
bool isEqualNote(Note a, Note b) {
  return a.pitch == b.pitch && a.volume == b.volume && a.wave == b.wave &&
         a.effect == b.effect;
}
bool isInvalid(Note symbol) { return symbol.id == -2; }

Note newNote(byte note, hex volume, WaveType wave, EffectType effect) {
  static int id = 0;

  return (Note){note, volume, wave, effect, id++};
}
Note newRest() { return (Note){0, 0, TRIANGLE, NONE, -1}; }
Note newInvalid() { return (Note){0, 0, TRIANGLE, NONE, -2}; }
