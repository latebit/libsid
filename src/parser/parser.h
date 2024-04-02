#ifndef PARSER_H
#define PARSER_H

typedef char *string;

typedef struct {
  int bpm;
  int subdivisions;
  int bars;
  int tracksCount;
  string **tracks;
} Composition;

Composition *fromFile(char *filename);
void freeComposition(Composition *c);

#endif