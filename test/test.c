#include "test.h"
#include <stdio.h>
#include <string.h>

#define abs(x) ((x) < 0 ? -(x) : (x))

int testCount = 0;
int failedCount = 0;

void expect(bool condition, const char *name, const char *message) {
  testCount++;
  if (!condition) {
    failedCount++;
    printf("fail - %s: %s\n", name, message);
    return;
  }
  printf("ok - %s\n", name);
}

void expectEqlNote(Note a, Note b, char *name) {
  char msg[256];
  sprintf(msg, "Expected (%d, %d, %d, %d) to equal (%d, %d, %d, %d)", a.pitch,
          a.volume, a.wave, a.effect, b.pitch, b.volume, b.wave, b.effect);
  expect(isEqualNote(a, b), name, msg);
}

void expectTrue(bool condition, char *name) {
  expect(condition, name, "Expected value to be true");
}

void expectNotNull(void *a, char *name) {
  expect(a != NULL, name, "Expected value not to be null");
}

void expectNull(void *a, char *name) {
  expect(a == NULL, name, "Expected value to be null");
}

void expectEqlInt(int a, int b, char *name) {
  char msg[256];
  sprintf(msg, "Expected %d to equal %d", a, b);
  expect(a == b, name, msg);
}

void expectEqlFloat(float a, float b, char *name) {
  float tolerance = 0.0001;
  char msg[256];
  sprintf(msg, "Expected %f to equal %f", a, b);
  expect(abs(a - b) < tolerance, name, msg);
}

void expectLessThanFloat(float a, float b, char *name) {
  char msg[256];
  sprintf(msg, "Expected %f < %f", a, b);
  expect(a < b, name, msg);
}

void expectEqlString(char *a, char *b, char *name) {
  char msg[256 + strlen(a) + strlen(b) + 2];
  sprintf(msg, "Expected '%s' to equal '%s'", a, b);
  expect(a == b, name, msg);
};

int report() {
  printf("\n%d assertions, %d failed\n", testCount, failedCount);
  return failedCount;
};
