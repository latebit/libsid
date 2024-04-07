#ifndef TEST_H
#define TEST_H

#include "../src/lib/synth/track.h"
// Used for the test macro
#include <stdio.h>

#define test(func)                                                             \
  do {                                                                         \
    printf("%s\n", #func);                                                     \
    func();                                                                    \
  } while (0)

void expectEqlInt(int a, int b, char *msg);
void expectEqlFloat(float a, float b, char *msg);
void expectEqlString(char *a, char *b, char *msg);
void expectLessThanFloat(float a, float b, char *msg);
void expectEqlNote(Note a, Note b, char *msg);
void expectTrue(bool condition, char *name);
void expectNull(void *a, char *msg);
void expectNotNull(void *a, char *msg);
void expect(bool condition, const char *name, const char *message);

int report();
#endif
