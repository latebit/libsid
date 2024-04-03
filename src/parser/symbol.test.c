#include "symbol.h"
#include "../lib/note.h"
#include <assert.h>

int main(void) {
  Note *n = parseStandardSymbol("C#40F0");
  assert(n->pitch == 12);
  assert(n->volume == 15);
  assert(n->effect == DROP);

  freeNote(n);
  return 0;
}