#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "utils.h"

#define WAVES 4

typedef enum { TRIANGLE, SQUARE, SAWTOOTH, NOISE } WaveType;
typedef enum { NONE, DROP, SLIDE, FADEIN, FADEOUT } EffectType;

typedef struct {
  EffectType type;
  float amount;
  float previous;
} Effect;

typedef struct {
  float currentStep;
  float stepSize;
  float volume;
  WaveType waveType;
  Effect effect;
} Oscillator;

Oscillator *newOscillator(float frequency);
float oscillate(Oscillator *oscillator);

void setNote(Oscillator *oscillator, byte note);
void setVolume(Oscillator *oscillator, float volume);
void setWave(Oscillator *oscillator, WaveType wave);
void setEffect(Oscillator *oscillator, EffectType effect);

float processFrequency(Effect *effect, float step);
float processVolume(Effect *effect, float sample);

#endif
