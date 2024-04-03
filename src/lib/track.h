#ifndef TRACK_H
#define TRACK_H

#include "note.h"
#include "utils.h"

typedef struct {
  Note *notes;
  byte length;
} Track;

Track *newTrack(Note *notes, byte length);

void freeTrack(Track *track);

#endif
