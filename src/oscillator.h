#ifndef OSCILLATOR_H
#define OSCILLATOR_H

extern float NOTE_TO_FREQUENCY[96];

typedef enum { TRIANGLE, SQUARE, SAWTOOTH } Wave;
#define WAVES 3

typedef struct {
  float currentStep;
  float stepSize;
  float volume;
  Wave wave;
} Oscillator;

Oscillator *newOscillator(float frequency);
float getNextSample(Oscillator *oscillator);
void setFrequency(Oscillator *oscillator, float frequency);
void setVolume(Oscillator *oscillator, float volume);
void setWave(Oscillator *oscillator, Wave wave);

#endif
