#include "track.h"

#include <stdlib.h>
#include <string.h>

Track *newTrack(Note *notes, byte length) {
  Track *track = malloc(sizeof(Track));
  track->notes = notes;
  track->length = length;
  return track;
}

void freeTrack(Track *track) {
  free(track->notes);
  track->notes = NULL;
  track->length = 0;
  free(track);
}
