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

void expectEqlInt(int a, int b, char *name) {
  char msg[50];
  sprintf(msg, "Expected %d to equal %d", a, b);
  expect(a == b, name, msg);
}

void expectEqlString(char *a, char *b, char *name) {
  char msg[31 + strlen(a) + strlen(b) + 2];
  sprintf(msg, "Expected '%s' to equal '%s'", a, b);
  expect(a == b, name, msg);
};

int report() {
  printf("\n%d assertions, %d failed\n", testCount, failedCount);
  return failedCount;
};
