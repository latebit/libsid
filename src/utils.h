#ifndef UTILS_H
#define UTILS_H

extern const int BUFFER_SIZE;
extern const int SAMPLE_RATE;

// A 0-255 integer
typedef unsigned char byte;

float clamp(float value, float min, float max);

#endif