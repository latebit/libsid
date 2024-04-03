#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "symbol.h"

Tune *fromFile(char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    return NULL;
  }

  Tune *tune = malloc(sizeof(Tune));

  tune->version = calloc(5, sizeof(char));
  fscanf(file, "#%4c#\n", tune->version);
  fscanf(file, "%d\n", &tune->bpm);
  fscanf(file, "%d\n", &tune->ticksPerBeat);
  fscanf(file, "%d\n", &tune->beatsCount);
  fscanf(file, "%d\n", &tune->tracksCount);

  int maxTrackLength = tune->beatsCount * tune->ticksPerBeat;

  char ***rawTracks = malloc(sizeof(char **) * tune->tracksCount);
  for (int i = 0; i < tune->tracksCount; i++) {
    rawTracks[i] = malloc(sizeof(char *) * maxTrackLength);
  }

  int trackSize[tune->tracksCount];
  bool shouldCollect[tune->tracksCount];
  for (int i = 0; i < tune->tracksCount; i++) {
    trackSize[i] = 0;
    shouldCollect[i] = true;
  }

  for (int i = 0; i < maxTrackLength; i++) {
    char *line = NULL;
    size_t len = 0;

    validate(getline(&line, &len, file) != -1, "Unexpected end of file\n");

    char *symbol = strtok(line, "|\n");
    for (int j = 0; j < tune->tracksCount; j++) {
      if (i == 0) {
        validate(!isContinueSymbolString(symbol),
                 "First symbol of a track cannot be a continue symbol\n");
      }

      if (!shouldCollect[j]) {
        symbol = strtok(NULL, "|\n");
        continue;
      }

      if (isEndOfTrackSymbolString(symbol)) {
        shouldCollect[j] = false;
        continue;
      }

      rawTracks[j][i] = malloc(7);
      strcpy(rawTracks[j][i], symbol);
      symbol = strtok(NULL, "|\n");
      trackSize[j]++;
    }

    free(line);
  }

  tune->tracks = malloc(sizeof(Track) * tune->tracksCount);
  for (int i = 0; i < tune->tracksCount; i++) {
    Note *symbols = malloc(sizeof(Note) * trackSize[i]);

    for (int j = 0; j < trackSize[i]; j++) {
      char *rawSymbol = rawTracks[i][j];

      if (isRestSymbolString(rawSymbol)) {
        symbols[j] = *newRest();
      } else if (isContinueSymbolString(rawSymbol)) {
        // This is validated above, it won't go out of bounds
        symbols[j] = symbols[j - 1];
      } else {
        symbols[j] = *parseStandardSymbol(rawSymbol);
      }

      free(rawTracks[i][j]);
    }

    tune->tracks[i] = newTrack(symbols, trackSize[i]);

    free(rawTracks[i]);
  }
  free(rawTracks);

  fclose(file);
  return tune;
}
