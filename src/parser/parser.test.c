#include "parser.h"
#include "../../test/test.h"

void simplest() {
  char *str =
      "#v0.1#\n90\n1\n2\n3\nC-4---|C-4---|C-4---\nC-4---|C-4---|C-4---\n";
  Tune *t = fromString(str);

  expectNotNull(t, "parses tune");
  expectEqlInt(t->bpm, 90, "parses bpm");
  expectEqlInt(t->ticksPerBeat, 1, "parses ticks per beat");
  expectEqlInt(t->beatsCount, 2, "parses beats");
  expectEqlInt(t->tracksCount, 3, "parses tracks count");

  Note n = newNote(48, 8, 0, 0);

  for (int i = 0; i < 3; i++) {
    expectEqlInt(t->tracks[i]->length, 2, "has correct track length");

    for (int j = 0; j < 2; j++) {
      expectEqlNote(t->tracks[i]->notes[j], n, "reads note correctly");
    }
  }

  freeTune(t);
}

void differentLengths() {
  char *str = "#v0.1#\n"
              "90\n"
              "1\n"
              "3\n"
              "3\n"
              "C-4---|C-4---|B-4---\n"
              "G-4---|      |C-4---\n"
              "E-4---|      |      \n";

  Tune *t = fromString(str);

  expectNotNull(t, "parses tune");
  expectEqlInt(t->tracks[0]->length, 3, "has correct track length (0)");
  expectEqlInt(t->tracks[1]->length, 1, "has correct track length (1)");
  expectEqlInt(t->tracks[2]->length, 2, "has correct track length (2)");

  expectEqlNote(get(t->tracks[0], 0), newNote(48, 8, 0, 0),
                "reads correct note (0,0)");
  expectEqlNote(get(t->tracks[0], 1), newNote(55, 8, 0, 0),
                "reads correct note (0,1)");
  expectEqlNote(get(t->tracks[0], 2), newNote(52, 8, 0, 0),
                "reads correct note (0,2)");
  expectEqlNote(get(t->tracks[1], 0), newNote(48, 8, 0, 0),
                "reads correct note (1,0)");

  freeTune(t);
}

void rests() {
  char *str = "#v0.1#\n"
              "90\n"
              "1\n"
              "3\n"
              "1\n"
              "C-4---\n"
              "------\n"
              "E-4---\n";

  Tune *t = fromString(str);

  expectNotNull(t, "parses tune");
  expectEqlNote(get(t->tracks[0], 0), newNote(48, 8, 0, 0),
                "has correct first note");
  expectEqlNote(get(t->tracks[0], 1), newRest(), "has correct pause");
  expectEqlNote(get(t->tracks[0], 2), newNote(52, 8, 0, 0), "has last note");

  freeTune(t);
}

void continues() {
  char *str = "#v0.1#\n"
              "90\n"
              "1\n"
              "3\n"
              "1\n"
              "C-4---\n"
              "......\n"
              "------\n";

  Tune *t = fromString(str);

  expectNotNull(t, "parses tune");
  Note n = newNote(48, 8, 0, 0);
  expectEqlNote(get(t->tracks[0], 0), n, "has correct first note");
  expectEqlNote(get(t->tracks[0], 1), n, "keeps correct note");
  expectEqlNote(get(t->tracks[0], 2), newRest(), "stops note");
  freeTune(t);

  str = "#v0.1#\n"
        "90\n"
        "1\n"
        "2\n"
        "1\n"
        "------\n"
        "......\n";
  t = fromString(str);
  expectEqlNote(get(t->tracks[0], 0), newRest(), "registers pause");
  expectEqlNote(get(t->tracks[0], 1), newRest(), "keeps the pause");

  freeTune(t);
}

void brokenHeader() {
  char *str = "#v0.1#\n"
              "90\n"
              "1\n"
              "3\n"
              "C-4---\n"
              "......\n"
              "------\n";
  Tune *t = fromString(str);
  expectNull(t, "does not parse with missing data");

  str = "#v0.1ff#\n"
        "90\n"
        "1\n"
        "3\n"
        "1\n"
        "C-4---\n"
        "......\n"
        "------\n";
  expectNull(t, "does not parse with invalid version");

  str = "#v0.1#\n"
        "false\n"
        "1\n"
        "3\n"
        "1\n"
        "C-4---\n"
        "......\n"
        "......\n";
  expectNull(t, "does not parse with invalid numbers");
}

void header() {
  char *str = "#v0.2#\n"
              "90\n"
              "1\n"
              "3\n"
              "1\n"
              "C-4---\n"
              "......\n"
              "------\n";
  Tune *t = fromString(str);
  expectNull(t, "does not parse the tune with invalid version");

  str = "#v0.1#\n"
        "0\n"
        "1\n"
        "3\n"
        "1\n"
        "C-4---\n"
        "......\n"
        "------\n";
  t = fromString(str);
  expectNull(t, "does not parse the tune with invalid bpm (too small)");

  str = "#v0.1#\n"
        "400\n"
        "1\n"
        "3\n"
        "1\n"
        "C-4---\n"
        "......\n"
        "------\n";
  t = fromString(str);
  expectNull(t, "does not parse the tune with invalid bpm (too big)");

  str = "#v0.1#\n"
        "90\n"
        "0\n"
        "3\n"
        "1\n"
        "C-4---\n"
        "......\n"
        "------\n";
  t = fromString(str);
  expectNull(t,
             "does not parse the tune with invalid ticks per beat (too small)");

  str = "#v0.1#\n"
        "90\n"
        "17\n"
        "3\n"
        "1\n"
        "C-4---\n"
        "......\n"
        "------\n";
  t = fromString(str);
  expectNull(t,
             "does not parse the tune with invalid ticks per beat (too big)");

  str = "#v0.1#\n"
        "90\n"
        "1\n"
        "0\n"
        "1\n"
        "C-4---\n"
        "......\n"
        "------\n";
  t = fromString(str);
  expectNull(t, "does not parse the tune with invalid beats count (too small)");

  str = "#v0.1#\n"
        "90\n"
        "1\n"
        "65\n"
        "1\n"
        "C-4---\n"
        "......\n"
        "------\n";
  t = fromString(str);
  expectNull(t, "does not parse the tune with invalid beats count (too big)");

  str = "#v0.1#\n"
        "90\n"
        "1\n"
        "3\n"
        "0\n"
        "C-4---\n"
        "......\n"
        "------\n";
  t = fromString(str);
  expectNull(t,
             "does not parse the tune with invalid tracks count (too small)");

  str = "#v0.1#\n"
        "90\n"
        "1\n"
        "3\n"
        "4\n"
        "C-4---\n"
        "......\n"
        "------\n";
  t = fromString(str);
  expectNull(t, "does not parse the tune with invalid tracks count (too big)");
}

int main() {
  test(simplest);
  test(differentLengths);
  test(rests);
  test(continues);
  test(brokenHeader);
  test(header);

  return report();
}