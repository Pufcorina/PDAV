#ifndef ppm_h
#define ppm_h

struct PPMPixel{
     unsigned int red,green,blue;
} PPMPixel;

struct PPMImage{
     int x, y;
     char format;
     struct PPMPixel *data;
} PPMImage;

struct Pixels{
  int width, height;
  char format;
  unsigned int** a;
  unsigned int** b;
  unsigned int** c;
} Pixels;

struct EncodedMatrix {
  int i, j;
  int size;
  unsigned int** matrix;
} EncodedMatrix;

struct BlocksImg {
  int width, height;
  char format;
  struct EncodedMatrix* a;
  struct EncodedMatrix* b;
  struct EncodedMatrix* c;
} BlocksImg;

#endif
