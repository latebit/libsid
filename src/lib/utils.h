#ifndef UTILS_H
#define UTILS_H

#define BUFFER_SIZE 4096
#define SAMPLE_RATE 44100

// A 0-255 integer
typedef unsigned char byte;
typedef byte hex;

// Clamp a float between a minimum and maximum
float clamp(float value, float min, float max);

#endif
