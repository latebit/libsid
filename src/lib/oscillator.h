#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "utils.h"

// This is used to initialize the wave table
#define WAVES 4

// Waves supported by the oscillator
typedef enum { TRIANGLE, SQUARE, SAWTOOTH, NOISE } WaveType;
// Effects supported by the oscillator
typedef enum { NONE, DROP, SLIDE, FADEIN, FADEOUT } EffectType;

// Effects modulate the frequency or volume of the oscillator
typedef struct {
  // The type of effect
  EffectType type;
  // How much the effect will change the frequency or volume
  float amount;
  // A value used to store the previous sample when effects need memory
  float previous;
} Effect;

typedef struct {
  // The current step in the wave table
  float currentStep;
  // The step size used to advance in the wave table
  float stepSize;
  // The volume of the oscillator
  float volume;
  // The wave type used to produce the sound
  // This also maps to the wave table
  WaveType waveType;
  // The effect applied to the oscillator to modulate the frequency or volume
  Effect effect;
} Oscillator;

// Allocates an oscillator setting the step size based on the frequency
Oscillator *newOscillator(float frequency);
// Frees the oscillator
void freeOscillator(Oscillator *oscillator);

// Advances the oscillator by one step returning the current sample
float oscillate(Oscillator *oscillator);

// Sets the pitch of the oscillator
void setPitch(Oscillator *oscillator, byte note);
// Sets the frequency of the oscillator
void setVolume(Oscillator *oscillator, float volume);
// Sets the wave of the oscillator
void setWave(Oscillator *oscillator, WaveType wave);
// Sets the effect of the oscillator
void setEffect(Oscillator *oscillator, EffectType effect);

// Modulates the frequency based on the current effect returning the adjusted
// step size
float processFrequency(Effect *effect, float step);
// Modulates the volume based on the current effect returning the adjusted
// volume
float processVolume(Effect *effect, float sample);

extern const Effect DROP_EFFECT;
extern const Effect SLIDE_EFFECT;
extern const Effect FADEIN_EFFECT;
extern const Effect FADEOUT_EFFECT;
extern const Effect NONE_EFFECT;

#endif
