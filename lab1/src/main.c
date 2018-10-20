#include "rw.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    printf("Invalid number of arguments!\n");
    return 1;
  }

  PPMImage *image;
  image = readPPM(argv[1]);
  writePPM("finalFile.ppm", image);

  return 0;
}
