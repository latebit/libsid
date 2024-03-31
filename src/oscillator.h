#ifndef OSCILLATOR_H
#define OSCILLATOR_H

extern const int SAMPLE_RATE;
extern float NOTE_TO_FREQUENCY[96];

typedef struct {
  float currentStep;
  float stepSize;
  float volume;
} Oscillator;

Oscillator *newOscillator(float frequency);
float getNextSample(Oscillator *oscillator);
void setFrequency(Oscillator *oscillator, float frequency);
void setVolume(Oscillator *oscillator, float volume);

#endif