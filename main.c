#include <SDL2/SDL.h>
#include <stdlib.h>
#include "oscillator.h"
#include "symbol.h"

const int BUFFER_SIZE = 4096;

enum WaveType {
  SINE,
  SQUARE,
  TRIANGLE,
  NOISE
};

typedef struct {
  char * sequence;
  enum WaveType waveType;  
} Track;

// Boundaries
#define MAX_TRACKS 4
#define MAX_TRACK_LENGTH 64
#define MAX_BPM 240
#define MIN_BPM 60

Oscillator trackOne;
Symbol e, c;
int count = 0;

void callback(void *userdata, Uint8 *stream, int len) {
  count = count % 20; 
  Symbol s = count > 9 ? e : c;
  for (int i = 0; i < len/sizeof(float); i++) {
    setFrequency(&trackOne, NOTE_TO_FREQUENCY[s.note]);
    ((float*)stream)[i] = getNextSample(&trackOne);
  }
  count++;
}

int init() {
  if (0!=SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS)) return 1;
  
  e = fromString("E-405...");
  c = fromString("C-409...");
  trackOne = newOscillator(NOTE_TO_FREQUENCY[c.note]);
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
