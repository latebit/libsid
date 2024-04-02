#include <SDL2/SDL.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../lib/oscillator.h"
#include "../lib/sequencer.h"
#include "../lib/track.h"
#include "../parser/parser.h"

Sequencer sequencer;

void callback(void *userdata, Uint8 *stream, int len) {
  (void)userdata;
  int samples = len / sizeof(float);

  for (int i = 0; i < samples; i++) {
    ((float *)stream)[i] = getNextSampleForChannel(&sequencer);
  }
}

void load(char *filename) {
  assert(access(filename, F_OK) == 0);

  Composition *composition = fromFile(filename);
  sequencer = *newSequencer(composition->bpm, composition->subdivisions);

  for (int i = 0; i < composition->tracksCount; i++) {
    Track *track = fromRawTrack(composition->tracks[i], composition->bars);
    Oscillator *oscillator = newOscillator(0);
    setTrack(&sequencer, i, track, oscillator);
  }
  freeComposition(composition);
}

int main(int argc, char *argv[]) {
  if (0 != SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS))
    return 1;

  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    exit(1);
  }

  load(argv[1]);

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
