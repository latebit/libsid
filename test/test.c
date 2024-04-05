#include "test.h"
#include <stdio.h>
#include <string.h>

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

void expectNotNull(void *a, char *name) {
  char msg[256];
  sprintf(msg, "Expected %s to be non-null", name);
  expect(a != NULL, name, msg);
}

void expectNull(void *a, char *name) {
  char msg[256];
  sprintf(msg, "Expected %s to be null", name);
  expect(a == NULL, name, msg);
}

void expectEqlInt(int a, int b, char *name) {
  char msg[256];
  sprintf(msg, "Expected %d to equal %d", a, b);
  expect(a == b, name, msg);
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
