#include "parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tune *fromFile(char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    return NULL;
  }

  Tune *tune = malloc(sizeof(Tune));

  // read the first line of the file
  fscanf(file, "%d\n", &tune->bpm);
  fscanf(file, "%d\n", &tune->subdivisions);
  fscanf(file, "%d\n", &tune->trackSize);
  fscanf(file, "%d\n", &tune->tracksCount);

  // allocate an array of array of strings
  tune->tracks = malloc(sizeof(char **) * tune->tracksCount);
  for (int i = 0; i < tune->tracksCount; i++) {
    tune->tracks[i] = malloc(sizeof(char *) * tune->trackSize);
  }

  // read the rest of the file
  for (int i = 0; i < tune->trackSize; i++) {
    char *line = NULL;
    size_t len = 0;

    assert(getline(&line, &len, file) != -1);

    char *symbol = strtok(line, " \t\n\r\f\v");
    for (int j = 0; j < tune->tracksCount; j++) {
      assert(strlen(symbol) > 0);
      tune->tracks[j][i] = (char *)malloc(strlen(symbol) + 1);
      strcpy(tune->tracks[j][i], symbol);
      symbol = strtok(NULL, " \t\n\r\f\v");
    }

    free(line);
  }

  fclose(file);
  return tune;
}

void freeTune(Tune *c) {
  for (int i = 0; i < c->tracksCount; i++) {
    for (int j = 0; j < c->trackSize; j++) {
      free(c->tracks[i][j]);
    }
    free(c->tracks[i]); // Free each array of strings
  }
  free(c->tracks);
}