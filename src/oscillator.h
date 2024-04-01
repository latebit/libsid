#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "utils.h"

typedef enum { TRIANGLE, SQUARE, SAWTOOTH, NOISE } Wave;
#define WAVES 4

typedef struct {
  float currentStep;
  float stepSize;
  float volume;
  Wave wave;
} Oscillator;

Oscillator *newOscillator(float frequency);
float getNextSample(Oscillator *oscillator);
void setNote(Oscillator *oscillator, byte note);
void setVolume(Oscillator *oscillator, float volume);
void setWave(Oscillator *oscillator, Wave wave);

#endif
