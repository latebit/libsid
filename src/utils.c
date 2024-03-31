const int BUFFER_SIZE = 4096;
const int SAMPLE_RATE = 44100;

float clamp(float value, float min, float max) {
  return value < min ? min : value > max ? max : value;
}