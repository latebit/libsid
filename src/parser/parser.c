#include "parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Composition *fromFile(char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    return NULL;
  }

  Composition *composition = malloc(sizeof(Composition));

  // read the first line of the file
  fscanf(file, "%d\n", &composition->bpm);
  fscanf(file, "%d\n", &composition->subdivisions);
  fscanf(file, "%d\n", &composition->bars);
  fscanf(file, "%d\n", &composition->tracksCount);

  // allocate an array of array of strings
  composition->tracks = malloc(sizeof(char **) * composition->tracksCount);
  for (int i = 0; i < composition->tracksCount; i++) {
    composition->tracks[i] = malloc(sizeof(char *) * composition->bars);
  }

  // read the rest of the file
  for (int i = 0; i < composition->bars; i++) {
    char *line = NULL;
    size_t len = 0;

    assert(getline(&line, &len, file) != -1);

    char *symbol = strtok(line, " \t\n\r\f\v");
    for (int j = 0; j < composition->tracksCount; j++) {
      assert(strlen(symbol) > 0);
      composition->tracks[j][i] = (char *)malloc(strlen(symbol) + 1);
      strcpy(composition->tracks[j][i], symbol);
      symbol = strtok(NULL, " \t\n\r\f\v");
    }

    free(line);
  }

  fclose(file);
  return composition;
}

void freeComposition(Composition *c) {
  for (int i = 0; i < c->tracksCount; i++) {
    for (int j = 0; j < c->bars; j++) {
      free(c->tracks[i][j]);
    }
    free(c->tracks[i]); // Free each array of strings
  }
  free(c->tracks);
}