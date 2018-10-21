#include "../header/matrixMan.h"
#include <stdlib.h>

unsigned int clampTo8Bit(int v) {
  if ( (v & ~0xFF) != 0 )
    v = ((~v) >> 31) & 0xFF;
  return v;
}

unsigned int** matrixMalloc(int width, int height) {
  unsigned int** matrix = (unsigned int**) malloc(sizeof(unsigned int*) * height);
  for (int i = 0; i < height; i++) {
    matrix[i] = (unsigned int*) malloc(sizeof(unsigned int) * width);
  }

  return matrix;
}
