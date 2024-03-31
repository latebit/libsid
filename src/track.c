#include "track.h"

#include <stdlib.h>
#include <string.h>

Track* fromRawTrack(char **rawTrack, int length) {
  Track* track = malloc(sizeof(Track));
  track->length = length;
  track->notes = malloc(track->length * sizeof(Symbol));

  Symbol last;
  for (int i = 0; i < track->length; i++) {
    if (strcmp("...", rawTrack[i]) == 0) {
      track->notes[i] = last;
      continue;
    };

    Symbol s = fromString(rawTrack[i]);
    track->notes[i] = s;
    last = s;
  }

  return track;
}

void freeTrack(Track *track) {
  free(track->notes);
  track->notes = NULL;
  track->length = 0;
}