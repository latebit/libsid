#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../lib/sequencer.h"
#include "../parser/parser.h"

Sequencer *sequencer;
Tune *tune;

void callback(void *userdata, Uint8 *stream, int len) {
  (void)userdata;
  int samples = len / sizeof(float);

  for (int i = 0; i < samples; i++) {
    ((float *)stream)[i] = getNextSampleForChannel(sequencer);
  }
}

void load(char *filename) {
  tune = fromFile(filename);
  sequencer = newSequencer(tune);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    char *name = basename(argv[0]);
    printf("Usage: %s <filename>\n", name);
    exit(1);
  }

  if (0 != SDL_Init(SDL_INIT_AUDIO)) {
    printf("SDL_Init: %s\n", SDL_GetError());
    exit(1);
  }

  load(argv[1]);

  SDL_AudioSpec spec = {.format = AUDIO_F32,
                        .channels = 1,
                        .freq = SAMPLE_RATE,
                        .samples = BUFFER_SIZE,
                        .callback = callback};

  unsigned int d = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 1);
  SDL_PauseAudioDevice(d, 0);

  SDL_Event e;
  while (SDL_WaitEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      freeSequencer(sequencer);
      return 0;
    }
  }

  freeSequencer(sequencer);

  return 0;
}
