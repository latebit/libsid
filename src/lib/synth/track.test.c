#include "track.h"
#include "../../../test/test.h"

void notes() {
  Note n = newNote(48, 8, 0, 0);
  Note m = newNote(48, 8, 0, 0);
  Note r = newRest();
  Note s = newRest();
  Note i = newInvalid();
  Note j = newInvalid();

  expectTrue(isSameNote(n, n), "compares same note");
  expectTrue(!isSameNote(n, m), "compares different notes");
  expectTrue(isEqualNote(n, m), "deep compares different notes");

  expectTrue(isSameNote(r, r), "compares same rest");
  expectTrue(isSameNote(r, s), "compares different rest");
  expectTrue(isRest(r), "detects rest");
  expectTrue(!isRest(i), "does not detect rest");
  expectTrue(!isRest(n), "does not detect rest");

  expectTrue(isSameNote(i, i), "compares same invalid");
  expectTrue(isSameNote(i, j), "compares different invalid");
  expectTrue(isInvalid(i), "detects invalid");
  expectTrue(!isInvalid(r), "does not detect invalid");
  expectTrue(!isInvalid(n), "does not detect invalid");
}

void items() {
  Track *t = newTrack(2);
  push(t, newRest());
  expectEqlInt(t->capacity, 2, "capacity doesn't change");
  expectEqlInt(t->length, 1, "length increases");

  expectEqlNote(
      get(t, 1), newInvalid(),
      "returns invalid when out of bounds (too high, below capacity)");
  expectEqlNote(
      get(t, 5), newInvalid(),
      "returns invalid when out of bounds (too high, above capacity)");
  expectEqlNote(get(t, -1), newInvalid(),
                "returns invalid when out of bounds (too low)");

  push(t, newRest());
  expectEqlInt(-1, push(t, newRest()), "fails pushing above the capacity");
  expectEqlInt(2, t->capacity, "capacity does not change");
  expectEqlInt(2, t->length, "length does not change");

  freeTrack(t);
}

void init() {
  Track *t = newTrack(10);

  expectEqlInt(t->capacity, 10, "allocates correct capacity");
  expectEqlInt(t->length, 0, "length is null");

  freeTrack(t);
}

int main(void) {
  test(init);
  test(items);
  test(notes);
  return report();
}
