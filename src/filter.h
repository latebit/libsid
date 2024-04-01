#ifndef FILTER_H
#define FILTER_H

typedef enum { LOWPASS, HIGHPASS, BANDPASS } FilterType;

typedef struct {
  FilterType type;
  float value;
} Filter;

#endif