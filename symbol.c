#include "symbol.h"

int hexToInt(char hexChar) {
  if (hexChar >= '0' && hexChar <= '9') {
    return hexChar - '0';
  } else if (hexChar >= 'a' && hexChar <= 'f') {
    return hexChar - 'a' + 10;
  } else if (hexChar >= 'A' && hexChar <= 'F') {
    return hexChar - 'A' + 10;
  }
  return 0;
}

int charToInt(char c) { return c - '0'; }

Symbol fromString(char * str) {
  unsigned char note = 0;
  unsigned char volume = 0;
  Effect effect = {0, 0};

  switch (str[0]) {
    case 'C': note = 12; break;
    case 'D': note = 2; break;
    case 'E': note = 4; break;
    case 'F': note = 5; break;
    case 'G': note = 7; break;
    case 'A': note = 9; break;
    case 'B': note = 11; break;
  }

  switch (str[1]) {
    case '#' : note++; break;
    case 'b' : note--; break;
  }

  note = (note % 12) + (12 * charToInt(str[2]));
  volume = hexToInt(str[4]);

  return (Symbol){note, volume, effect};
}