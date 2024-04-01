#ifndef TRACK_H
#define TRACK_H

#include "symbol.h"
#include "utils.h"

typedef struct {
  Symbol *notes;
  byte length;
} Track;

// Creates a Track from a tracker entry
Track *fromRawTrack(char **rawTrack, int length);
void freeTrack(Track *track);

#endif
