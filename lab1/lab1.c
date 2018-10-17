#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct pixelsRGB pixelsRGB;

struct pixelsRGB {
  int** r;
  int** g;
  int** b;
};

typedef struct pixelsYUV pixelsYUV;

typedef struct pixelsYUV {
  int** y;
  int** u;
  int** v;
};

void writeInFile() {

}

int** matrixMalloc(int width, int height) {
  int** matrix = (int**) malloc(sizeof(int*) * height);
  for (int i = 0; i < height; i++) {
    matrix[i] = (int*) malloc(sizeof(int) * width);
  }

  return matrix;
}

pixelsRGB readFromFile(const char* fileName, char* pSize,  char* description, int* width, int* height, int* maximum) {
  FILE* fr = fopen(fileName, "r");
  pixelsRGB rgb;

  pSize = (char*)malloc(3);
  description = (char*)malloc(10000);

  // formatted read of header
  fscanf(fr, "%s\n", pSize);
  fscanf(fr, "%9999[^\n]", description);

  if (strncmp(pSize, "P3", 2) == 0)
  {
    // read the rest of the header
    fscanf(fr, "%d %d\n %d\n", width, height, maximum);

    rgb.r = matrixMalloc(*width, *height);
    rgb.g = matrixMalloc(*width, *height);
    rgb.b = matrixMalloc(*width, *height);

    int colorPixel;
    int l = 0;
    int c = 0;

    for (int i = 0; i < (*width) * (*height); i++ )
    {
      if ( c == *width ) {
        c = 0;
        l++;
      }

      fscanf(fr, "%d\n", &colorPixel);

      if ( i % 3 == 0 ) rgb.r[l][c] = colorPixel;
      if ( i % 3 == 1 ) rgb.g[l][c] = colorPixel;
      if ( i % 3 == 2 ) rgb.b[l][c] = colorPixel;

      c++;
    }

    fclose(fr);
    return rgb;
  }
  else
    printf("Not a PPM format\n");
}

pixelsYUV convertFromRGBtoYUV(int width, int height, pixelsRGB rgb) {
  pixelsYUV yuv;

  yuv.y = matrixMalloc(width, height);
  yuv.u = matrixMalloc(width, height);
  yuv.v = matrixMalloc(width, height);

  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
    {
      yuv.y[i][j] = 0.299 * rgb.r[i][j] + 0.587 * rgb.g[i][j] + 0.114 * rgb.b[i][j];
      yuv.u[i][j] = 128 - 0.1687 * rgb.r[i][j] - 0.3312 * rgb.g[i][j] + 0.5 * rgb.b[i][j];
      yuv.v[i][j] = 128 + 0.5 * rgb.r[i][j] - 0.4186 * rgb.g[i][j] - 0.0813 * rgb.b[i][j];
    }

    return yuv;
}

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("Invalid number of arguments!\n");
    return 1;
  }

  if (access(argv[1], F_OK) == -1) {
    printf("Invalid file!\n");
    return 2;
  }

  char* pSize;
  char* description;
  int width, height, maximum;

  pixelsRGB rgb;
  pixelsYUV yuv;

  // encoder pat
  rgb = readFromFile(argv[1], pSize, description, &width, &height, &maximum);
  yuv = convertFromRGBtoYUV(width, height, rgb);

  // decoder part
  return 0;
}
