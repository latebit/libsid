#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "symbol.h"

char *readFileIntoBuffer(char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    return NULL;
  }

  // Get the file size
  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Read the file into the buffer
  char *buffer = malloc(fsize + 1);
  fread(buffer, 1, fsize, file);
  buffer[fsize] = 0;

  fclose(file);

  return buffer;
}

// Reads the header of the tune and returns the offset to the end of the header
// or -1 if the header is invalid
int readHeader(char *str, char *version, int *bpm, int *ticksPerBeat,
               int *beatsCount, int *tracksCount) {
  int offset = 0;
  int read = sscanf(str, "#%4c#\n%d\n%d\n%d\n%d\n%n", version, bpm,
                    ticksPerBeat, beatsCount, tracksCount, &offset);
  if (read != 5) {
    error("Invalid header");
    return -1;
  }

  return offset;
}

Tune *fromString(char *str) {
  char version[5] = "";
  int bpm, ticksPerBeat, beatsCount, tracksCount;
  int headerOffset =
      readHeader(str, version, &bpm, &ticksPerBeat, &beatsCount, &tracksCount);

  if (headerOffset < 0) {
    error("Invalid header");
    return NULL;
  }

  if (strcmp(version, "v0.1") != 0) {
    error("Unsupported version %s", version);
    return NULL;
  }

  Tune *tune = newTune(tracksCount);
  tune->bpm = bpm;
  tune->ticksPerBeat = ticksPerBeat;
  tune->beatsCount = beatsCount;

  int maxTrackLength = tune->beatsCount * tune->ticksPerBeat;

  // A flag use to stop collection of symbols for a track when the end of track
  // symbol is found
  bool trackEnded[tune->tracksCount];

  // Initialize the tracks and track ended flags
  for (int i = 0; i < tune->tracksCount; i++) {
    tune->tracks[i] = newTrack(maxTrackLength);
    trackEnded[i] = false;
  }

  // A string containing the body of the tune
  char *body = strdup(str + headerOffset);

  // Pointers used by strtok_r to keep track of the current line and symbol
  // Note: they cannot be freed, they are managed by strtok_r
  char *lineOffset, *symbolOffset;

  // The current line being processed
  char *line = strtok_r(body, "\n", &lineOffset);

  for (int i = 0; i < maxTrackLength; i++) {
    if (!line) {
      error("Unexpected end of file");
      goto cleanAndReturn;
    };

    char *symbol = strtok_r(line, "|\n", &symbolOffset);
    for (int j = 0; j < tune->tracksCount; j++) {
      if (trackEnded[j]) {
        symbol = strtok_r(NULL, "|\n", &symbolOffset);
        continue;
      }

      if (isEndOfTrackSymbolString(symbol)) {
        symbol = strtok_r(NULL, "|\n", &symbolOffset);
        trackEnded[j] = true;
        continue;
      }

      Track *track = tune->tracks[j];

      if (isRestSymbolString(symbol)) {
        push(track, newRest());
      } else if (isContinueSymbolString(symbol)) {
        if (i == 0) {
          error("First symbol of a track cannot be a continue symbol");
          goto cleanAndReturn;
        }

        push(track, get(track, i - 1));
      } else {
        Note n = parseStandardSymbol(symbol);
        if (isInvalid(n)) {
          // The error message is already printed in the symbol parser
          goto cleanAndReturn;
        }

        push(track, n);
      }

      symbol = strtok_r(NULL, "|\n", &symbolOffset);
    }

    line = strtok_r(NULL, "\n", &lineOffset);
  }
  free(body);

  return tune;

cleanAndReturn:
  free(line);
  free(body);
  freeTune(tune);
  return NULL;
}

Tune *fromFile(char *filename) {
  char *content = readFileIntoBuffer(filename);
  if (content == NULL) {
    error("Could not open file %s", filename);
    return NULL;
  }

  Tune *tune = fromString(content);
  free(content);

  return tune;
}