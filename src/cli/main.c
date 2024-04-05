#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../lib/oscillator.h"
#include "../lib/sequencer.h"
#include "../parser/parser.h"

Sequencer *sequencer;
Tune *tune;
int loaded = 0;

void callback(void *userdata, Uint8 *stream, int len) {
  (void)userdata;
  int samples = len / sizeof(float);

  if (loaded != 5) {
    loaded++;
    SDL_memset(stream, 0, len);
    return;
  }

  for (int i = 0; i < samples; i++) {
    ((float *)stream)[i] = getNextSampleForChannel(sequencer);
  }
}

void load(char *filename) {
  tune = fromFile(filename);
  sequencer = newSequencer(tune->bpm, tune->ticksPerBeat);

  for (int i = 0; i < tune->tracksCount; i++) {
    Oscillator *oscillator = newOscillator(0);
    setTrack(sequencer, i, tune->tracks[i], oscillator);
  }
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

  unsigned int d = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 1);
  SDL_PauseAudioDevice(d, 0);

  SDL_Event e;
  while (SDL_WaitEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      freeTune(tune);
      freeSequencer(sequencer);
      return 0;
    }
  }

  freeSequencer(sequencer);
  freeTune(tune);

  return 0;
}
