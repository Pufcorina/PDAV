#include "../header/encoder.h"
#include "../header/decoder.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, const char *argv[]) {
  if (argc != 3) {
    printf("Invalid number of arguments!\n");
    return 1;
  }

  decode_ppm(encode_ppm(argv[1]), argv[2]);
  printf("Imi cer scuze pentru orice maltratare, totul a fost in scop stiintific :(\n");

  return 0;
}
