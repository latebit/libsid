#include "symbol.h"
#include "../../../test/test.h"
#include <stdio.h>

void alterations() {
  Note n;
  n = parseStandardSymbol("C#51F0");
  expectEqlInt(n.pitch, 61, "parses sharp");

  n = parseStandardSymbol("Eb51F0");
  expectEqlInt(n.pitch, 63, "parses flat");

  n = parseStandardSymbol("B#51F0");
  expectEqlInt(n.pitch, 72, "parses sharp (boundary)");

  n = parseStandardSymbol("Cb51F0");
  expectEqlInt(n.pitch, 59, "parses flat (boundary)");
}

void defaultValues() {
  Note n;

  n = parseStandardSymbol("--50F0");
  expect(isRest(n), "default pitch is rest",
         "Expected default pitch to be rest.");

  n = parseStandardSymbol("C-50F0");
  expectEqlInt(n.pitch, 60, "default alteration is none");

  n = parseStandardSymbol("C#-0F0");
  expectEqlInt(n.pitch, 49, "default octave is 4");

  n = parseStandardSymbol("C#5-F0");
  expectEqlInt(n.wave, TRIANGLE, "default wave is TRIANGLE");

  n = parseStandardSymbol("C#51-0");
  expectEqlInt(n.volume, 8, "default volume is 8");

  n = parseStandardSymbol("C#519-");
  expectEqlInt(n.effect, NONE, "default volume is NONE");
}

void effects() {
  Note n;

  n = parseStandardSymbol("C#51F0");
  expectEqlInt(n.effect, NONE, "parses effect NONE");
  n = parseStandardSymbol("C#51F1");
  expectEqlInt(n.effect, DROP, "parses effect DROP");
  n = parseStandardSymbol("C#51F2");
  expectEqlInt(n.effect, SLIDE, "parses effect SLIDE");
  n = parseStandardSymbol("C#51F3");
  expectEqlInt(n.effect, FADEIN, "parses effect FADEIN");
  n = parseStandardSymbol("C#51F4");
  expectEqlInt(n.effect, FADEOUT, "parses effect FADEOUT");
}

void volume() {
  Note n;
  n = parseStandardSymbol("C#51F0");
  expectEqlInt(n.volume, 15, "parses volume 15");

  n = parseStandardSymbol("C#51E1");
  expectEqlInt(n.volume, 14, "parses volume 14");

  n = parseStandardSymbol("C#5192");
  expectEqlInt(n.volume, 9, "parses volume 9");
}

void waves() {
  Note n;
  n = parseStandardSymbol("C#50F0");
  expectEqlInt(n.wave, TRIANGLE, "parses TRIANGLE");

  n = parseStandardSymbol("C#51F0");
  expectEqlInt(n.wave, SAWTOOTH, "parses SAWTOOTH");

  n = parseStandardSymbol("C#52F0");
  expectEqlInt(n.wave, SQUARE, "parses SQUARE");

  n = parseStandardSymbol("C#53F0");
  expectEqlInt(n.wave, NOISE, "parses NOISE");
}

void octaves() {
  Note n;
  for (int i = 0; i < 8; i++) {
    char symbol[7], name[16];
    sprintf(symbol, "C#%d0F0", i);
    sprintf(name, "parses octave %d", i);
    n = parseStandardSymbol(symbol);
    expectEqlInt(n.pitch, 1 + (12 * i), name);
  }
}

void invalid() {
  Note n;
  n = parseStandardSymbol("");
  expect(isInvalid(n), "empty string", "Expected NULL for empty string.");

  n = parseStandardSymbol("C#51F");
  expect(isInvalid(n), "incomplete string",
         "Expected NULL for incomplete string.");

  n = parseStandardSymbol("C#51F5");
  expect(isInvalid(n), "invalid effect", "Expected NULL for invalid effect.");

  n = parseStandardSymbol("C#51E5");
  expect(isInvalid(n), "invalid volume", "Expected NULL for invalid volume.");

  n = parseStandardSymbol("C#51D5");
  expect(isInvalid(n), "invalid wave", "Expected NULL for invalid wave.");

  n = parseStandardSymbol("C#51C5");
  expect(isInvalid(n), "invalid pitch", "Expected NULL for invalid pitch.");

  n = parseStandardSymbol("C#51B5");
  expect(isInvalid(n), "invalid alteration",
         "Expected NULL for invalid alteration.");
}

int main(void) {
  Note n = parseStandardSymbol("C#51F1");
  expectEqlInt(n.pitch, 61, "uses correct pitch");
  expectEqlInt(n.wave, SAWTOOTH, "uses correct wave");
  expectEqlInt(n.volume, 15, "uses correct volume");
  expectEqlInt(n.effect, DROP, "uses correct effect");

  defaultValues();
  alterations();
  octaves();
  waves();
  volume();
  effects();
  invalid();

  return report();
}
