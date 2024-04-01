#ifndef PARSER_H
#define PARSER_H

typedef struct {
  int bpm;
  int subdivisions;
  int bars;
  int tracksCount;
  char ***tracks;
} Composition;

Composition *fromFile(char *filename);

#endif