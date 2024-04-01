#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include "src/oscillator.h"
#include "src/sequencer.h"
#include "src/track.h"

Sequencer sequencer;

void callback(void * userdata, Uint8 *stream, int len) {
  (void)userdata;
  int samples = len / sizeof(float);

  for (int i = 0; i < samples; i++) {
    ((float*)stream)[i] = getNextSampleForChannel(&sequencer);
  }
}

int init(void) {
  if (0!=SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS)) return 1;
  
  char* rawTrackOne[16] = {
    "C-405---", 
    "...", 
    "...", 
    "...", 
    "E-405---", 
    "...", 
    "G-405---", 
    "...", 
    "E-405---", 
    "...", 
    "G-405---", 
    "...", 
    "B-405---", 
    "...", 
    "C-505---", 
    "..."};

  char* rawTrackTwo[16] = {
    "C-202---", 
    "...", 
    "C-302---",
    "...", 
    "C-202---", 
    "...", 
    "C-302---",
    "...",
    "C-202---", 
    "...", 
    "C-302---",
    "...",
    "C-202---", 
    "...", 
    "C-302---",
    "..."};

  char* rawTrackThree[16] = {
    "C-102---", 
    "--------", 
    "--------",
    "--------", 
    "B-702---", 
    "--------", 
    "--------",
    "--------",
    "C-102---", 
    "--------", 
    "--------",
    "--------",
    "C-102---", 
    "--------", 
    "--------",
    "--------"};

  Track * trackOne = fromRawTrack(rawTrackOne, 16);
  Track * trackTwo = fromRawTrack(rawTrackTwo, 16);
  Track * trackThree = fromRawTrack(rawTrackThree, 16);
  Oscillator *oscillatorOne = newOscillator(0);
  Oscillator *oscillatorTwo = newOscillator(0);
  Oscillator *oscillatorThree = newOscillator(0);
  setWave(oscillatorOne, TRIANGLE);
  setWave(oscillatorTwo, SAWTOOTH);
  setWave(oscillatorThree, NOISE);
  
  sequencer = *newSequencer(100, 4);
  setTrack(&sequencer, 0, trackOne, oscillatorOne);
  setTrack(&sequencer, 1, trackTwo, oscillatorTwo);
  setTrack(&sequencer, 2, trackThree, oscillatorThree);

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
