#include "src/oscillator.h"
#include "src/parser.h"
#include "src/sequencer.h"
#include "src/track.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Sequencer sequencer;

void callback(void *userdata, Uint8 *stream, int len) {
  (void)userdata;
  int samples = len / sizeof(float);

  for (int i = 0; i < samples; i++) {
    ((float *)stream)[i] = getNextSampleForChannel(&sequencer);
  }
}

int init(void) {
  if (0 != SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS))
    return 1;

  Composition *composition = fromFile("example");
  sequencer = *newSequencer(composition->bpm, composition->subdivisions);

  for (int i = 0; i < composition->tracksCount; i++) {
    Track *track = fromRawTrack(composition->tracks[i], composition->bars);
    Oscillator *oscillator = newOscillator(0);
    setTrack(&sequencer, i, track, oscillator);
  }

  return 0;
}

int main(void) {
  if (0 != init())
    return 1;

  SDL_AudioSpec spec = {.format = AUDIO_F32,
                        .channels = 1,
                        .freq = SAMPLE_RATE,
                        .samples = BUFFER_SIZE,
                        .callback = callback};

  if (0 != SDL_OpenAudio(&spec, NULL))
    return 1;

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
