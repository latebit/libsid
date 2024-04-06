#ifndef TRACK_H
#define TRACK_H

#include "oscillator.h"
#include "utils.h"

// A Note is a musical symbol that represents a sound. It has a pitch, a volume,
// a wave type and an effect type. It also has a unique identifier that can be
// used to compare notes.
// A note can also be a rest, which is a symbol that represents silence.
// A note can also be invalid, which is a symbol that represents an invalid note
// and it is used to report errors.
typedef struct {
  // A number between 0 and 96 describing a note from C0 to B7
  byte pitch;
  // A number between 0 and 15 describing the volume of the note
  hex volume;
  // The type of wave associated with the note
  WaveType wave;
  // The type of effect associated with the note
  EffectType effect;
  // The unique identifier of the note, used to compare notes
  long unsigned int id;
} Note;

// A track is a tiny wrapper around an array of notes. It has a length, a
// capacity and a pointer to the note array.
typedef struct {
  // An array of Notes
  Note *notes;
  // The number of notes in the track
  byte length;
  // The maximum number of notes that can be stored in the track
  byte capacity;
} Track;

// Creates a new track with the given length
Track *newTrack(byte length);
// Frees the memory allocated for the track
void freeTrack(Track *track);
// Pushes a note into the track with bounds checking
int push(Track *track, Note note);
// Gets the note at the given index with bounds checking. Returns an invalid
// note in case of error
Note get(Track *track, byte index);

// Creates a new note with the given pitch, volume, wave and effect.
// Warning: This is the only constructor that doesn't do heap allocation by
// default.
Note newNote(byte pitch, hex volume, WaveType wave, EffectType effect);
// Creates a new rest note
Note newRest();
// Creates a new invalid note
Note newInvalid();

// Checks if the given note is a rest
bool isRest(Note symbol);
// Checks if the given notes are the same (i.e., have the same id)
bool isSameNote(Note a, Note b);
// Checks if the given notes are equal (i.e., have the same pitch, volume, wave)
bool isEqualNote(Note a, Note b);
// Checks if the given note is invalid
bool isInvalid(Note symbol);

#endif
