#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include "oscillator.h"
#include "symbol.h"

const int BUFFER_SIZE = 4096;

// A 0-255 integer
typedef unsigned char byte;

// Boundaries
#define MAX_TRACKS 4
#define MAX_TRACK_LENGTH 64
#define MAX_BPM 240
#define MIN_BPM 60

typedef struct {
  byte attack;
  byte decay;
  byte sustain;
  byte release;
} Modulator;

typedef struct {
  Modulator modulator;
  Symbol* notes;
  byte length;
} Track;

Oscillator oscillator;
Track track = {
  .modulator = {0, 0, 0, 0},
  .notes = {},
  .length = 16
};

// How many SDL ticks one of our ticks is, this should be calculated using bpms
const int tickLength = 20;
// How many SDL ticks have passed
int ticks = 0;
// How many of our ticks have passed
int subdivisions = 0;

void callback(void *userdata, Uint8 *stream, int len) {
  Symbol s = track.notes[subdivisions % track.length];
  setFrequency(&oscillator, NOTE_TO_FREQUENCY[s.note]);

  for (int i = 0; i < len/sizeof(float); i++) {
    ((float*)stream)[i] = getNextSample(&oscillator);
  }

  ticks++;
  if (ticks % tickLength == 0) {
    subdivisions++;
  }
}

int init() {
  if (0!=SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS)) return 1;
  
  char* rawTrack[16] = {
    "C-405---", 
    "...", 
    "...", 
    "...", 
    "E-405---", 
    "...", 
    "G-401---", 
    "...", 
    "E-405---", 
    "...", 
    "G-401---", 
    "...", 
    "B-407---", 
    "...", 
    "C-505---", 
    "..."};

  track.notes = malloc(track.length * sizeof(Symbol));
  Symbol last;
  for (int i = 0; i < 16; i++) {
    if (strcmp("...", rawTrack[i]) == 0) {
      track.notes[i] = last;
      continue;
    };

    Symbol s = fromString(rawTrack[i]);
    track.notes[i] = s;
    last = s;
  }

  oscillator = newOscillator(0);
  return 0;
}

int main(void) {
  if (0!=init()) return 1;

  SDL_AudioSpec spec = {
    .format = AUDIO_F32,
    .channels = 1,
    .freq = SAMPLE_RATE,
    .samples = BUFFER_SIZE,
    .callback = callback
  };

  if (0!=SDL_OpenAudio(&spec, NULL)) return 1;
  
  SDL_PauseAudio(0);

  while (1) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        return 0;
      }
    }
  }

  return 0;
}
