typedef struct {
     unsigned int red,green,blue;
} PPMPixel;

typedef struct {
     int x, y;
     char format;
     PPMPixel *data;
} PPMImage;

typedef struct {
  int** r;
  int** g;
  int** b;
} pixelsRGB;

typedef struct pixelsYUV {
  int** y;
  int** u;
  int** v;
} pixelsYUV;
