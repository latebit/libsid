#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include "src/oscillator.h"
#include "src/sequencer.h"
#include "src/track.h"

Sequencer sequencer;

void callback(void *userdata, Uint8 *stream, int len) {
  for (int i = 0; i < len/sizeof(float); i++) {
    ((float*)stream)[i] = getNextSampleForChannel(&sequencer, 0);
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

  Track * track = fromRawTrack(rawTrack, 16);
  Oscillator *oscillator = newOscillator(0);
  
  sequencer = *newSequencer(100, 4);
  setTrack(&sequencer, 0, track, oscillator);

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
