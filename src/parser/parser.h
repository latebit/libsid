#ifndef PARSER_H
#define PARSER_H

typedef char *string;

typedef struct {
  int bpm;
  int subdivisions;
  int trackSize;
  int tracksCount;
  string **tracks;
} Tune;

Tune *fromFile(char *filename);
void freeTune(Tune *c);

#endif