#include "oscillator.h"
#include "../../../test/test.h"
#include "utils.h"

void init() {
  Oscillator *o = newOscillator(440);
  expectEqlInt(o->currentStep, 0, "initial step should be 0");
  expectEqlFloat(o->stepSize, 40.867119, "sets correct stepSize");
  expectEqlFloat(o->volume, 0.5, "volume should be 0.5");
  expectEqlInt(o->waveType, TRIANGLE, "waveType should be TRIANGLE");
  freeOscillator(o);
}

void pitch() {
  Oscillator *o = newOscillator(100);
  setPitch(o, 69);
  expectEqlInt(o->currentStep, 0, "resets step to 0");
  expectEqlFloat(o->stepSize, 81.734240, "sets correct stepSize");
  freeOscillator(o);
}

void effect() {
  Oscillator *o = newOscillator(100);
  setEffect(o, DROP);
  expectEqlInt(o->effect.type, DROP, "sets effect type to DROP");
  expectEqlFloat(o->effect.amount, DROP_EFFECT.amount, "sets amount");
  expectEqlFloat(o->effect.previous, DROP_EFFECT.previous, "sets previous");
  freeOscillator(o);

  o = newOscillator(100);
  setEffect(o, SLIDE);
  expectEqlInt(o->effect.type, SLIDE, "sets effect type to SLIDE");
  expectEqlFloat(o->effect.amount, SLIDE_EFFECT.amount, "sets amount");
  expectEqlFloat(o->effect.previous, SLIDE_EFFECT.previous, "sets previous");
  freeOscillator(o);

  o = newOscillator(100);
  setEffect(o, FADEIN);
  expectEqlInt(o->effect.type, FADEIN, "sets effect type to FADEIN");
  expectEqlFloat(o->effect.amount, FADEIN_EFFECT.amount, "sets amount");
  expectEqlFloat(o->effect.previous, FADEIN_EFFECT.previous, "sets previous");
  freeOscillator(o);

  o = newOscillator(100);
  setEffect(o, FADEOUT);
  expectEqlInt(o->effect.type, FADEOUT, "sets effect type to FADEOUT");
  expectEqlFloat(o->effect.amount, FADEOUT_EFFECT.amount, "sets amount");
  expectEqlFloat(o->effect.previous, FADEOUT_EFFECT.previous, "sets previous");
  freeOscillator(o);

  o = newOscillator(100);
  setEffect(o, NONE);
  expectEqlInt(o->effect.type, NONE, "sets effect type to NONE");
  expectEqlFloat(o->effect.amount, NONE_EFFECT.amount, "sets amount");
  expectEqlFloat(o->effect.previous, NONE_EFFECT.previous, "sets previous");
  freeOscillator(o);
}

void frequency() {
  Effect e = DROP_EFFECT;
  // Applying one more time due to float precision
  processFrequency(&e, 100);
  float result[2] = {processFrequency(&e, 100), processFrequency(&e, 100)};
  expectLessThanFloat(result[0], 100, "reduces the frequency");
  expectLessThanFloat(result[1], result[0], "reduces the frequency");

  e = SLIDE_EFFECT;
  // Applying one more time due to float precision
  processFrequency(&e, 100);
  float result2[2] = {processFrequency(&e, 100), processFrequency(&e, 100)};
  expectLessThanFloat(100, result2[0], "increases the frequency");
  expectLessThanFloat(result2[0], result2[1], "increases the frequency");

  e = FADEIN_EFFECT;
  float result3[2] = {processFrequency(&e, 100), processFrequency(&e, 100)};
  expectEqlFloat(result3[0], 100, "doesn't change the frequency");
  expectEqlFloat(result3[1], result3[0], "doesn't change the frequency");

  e = FADEOUT_EFFECT;
  float result4[2] = {processFrequency(&e, 100), processFrequency(&e, 100)};
  expectEqlFloat(result4[0], 100, "doesn't change the frequency");
  expectEqlFloat(result4[1], result4[0], "doesn't change the frequency");

  e = NONE_EFFECT;
  float result5[2] = {processFrequency(&e, 100), processFrequency(&e, 100)};
  expectEqlFloat(result5[0], 100, "doesn't change the frequency");
  expectEqlFloat(result5[1], result5[0], "doesn't change the frequency");
}

void volume() {
  Effect e = FADEIN_EFFECT;
  float result[3] = {processVolume(&e, 0.5), processVolume(&e, 0.5),
                     processVolume(&e, 0.5)};
  expectLessThanFloat(result[0], result[1], "increases the volume");
  expectLessThanFloat(result[1], result[2], "increases the volume");

  e = FADEOUT_EFFECT;
  // Applying one more time due to float precision
  float result2[3] = {processVolume(&e, 0.5), processVolume(&e, 0.5),
                      processVolume(&e, 0.5)};
  expectLessThanFloat(result2[1], 0.5, "decreases the volume");
  expectLessThanFloat(result2[2], result2[1], "decreases the volume");

  e = NONE_EFFECT;
  float result3[2] = {processVolume(&e, 0.5), processVolume(&e, 0.5)};
  expectEqlFloat(result3[0], 0.5, "doesn't change the volume");
  expectEqlFloat(result3[1], result3[0], "doesn't change the volume");

  e = DROP_EFFECT;
  float result4[2] = {processVolume(&e, 0.5), processVolume(&e, 0.5)};
  expectEqlFloat(result4[0], 0.5, "doesn't change the volume");
  expectEqlFloat(result4[1], result4[0], "doesn't change the volume");

  e = SLIDE_EFFECT;
  float result5[2] = {processVolume(&e, 0.5), processVolume(&e, 0.5)};
  expectEqlFloat(result5[0], 0.5, "doesn't change the volume");
  expectEqlFloat(result5[1], result5[0], "doesn't change the volume");
}

void oscillations() {
  // Purposely chosing a slow frequency so that we don't have to deal with
  // periodicity
  const int FREQUENCY = 100;
  Oscillator *o1 = newOscillator(FREQUENCY);
  Oscillator *o2 = newOscillator(FREQUENCY);
  setEffect(o1, SLIDE);

  for (int i = 0; i < BUFFER_SIZE; i++) {
    oscillate(o1);
    oscillate(o2);
  }

  expectLessThanFloat(oscillate(o1), oscillate(o2),
                      "applies effect to frequency");
  freeOscillator(o1);
  freeOscillator(o2);

  o1 = newOscillator(FREQUENCY);
  o2 = newOscillator(FREQUENCY);
  setEffect(o1, FADEIN);

  for (int i = 0; i < BUFFER_SIZE; i++) {
    oscillate(o1);
    oscillate(o2);
  }

  expectLessThanFloat(oscillate(o1), oscillate(o2), "applies effect to volume");
  freeOscillator(o1);
  freeOscillator(o2);
}

int main(void) {
  test(init);
  test(pitch);
  test(effect);
  test(frequency);
  test(volume);
  test(oscillations);
  return report();
}
