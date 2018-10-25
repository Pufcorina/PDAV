#include "../header/matrixMan.h"
#include <stdlib.h>

unsigned int** matrixMalloc(int width, int height) {
  unsigned int** matrix = (unsigned int**) malloc(sizeof(unsigned int*) * height);
  for (int i = 0; i < height; i++) {
    matrix[i] = (unsigned int*) malloc(sizeof(unsigned int) * width);
  }

  return matrix;
}

unsigned int clampTo8Bit(int n){
    if (n < 0)
      n = 0;
    if (n > 255)
      n = 255;
    return n;
}
