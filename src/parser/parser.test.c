#include "parser.h"
#include <assert.h>

int main() {
  Tune *composition = fromFile("../examples/example");
  assert(!!composition);
  assert(composition->bpm == 120);
  assert(composition->subdivisions == 4);
  assert(composition->trackSize == 16);
  assert(composition->tracksCount == 3);

  freeTune(composition);
  return 0;
}