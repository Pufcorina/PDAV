#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {
     unsigned int red,green,blue;
} PPMPixel;

typedef struct {
     int x, y;
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

typedef struct {
  int*** matrixes;
} encodedMatrix;

#define CREATOR "Cwori"
#define RGB_COMPONENT_COLOR 255


int** matrixMalloc(int width, int height) {
  int** matrix = (int**) malloc(sizeof(int*) * height);
  for (int i = 0; i < height; i++) {
    matrix[i] = (int*) malloc(sizeof(int) * width);
  }

  return matrix;
}

// ppmFile readFromFile(const char* fileName) {
//   FILE* fr = fopen(fileName, "rb");
//   if (!fr) {
//        fprintf(stderr, "Unable to open file '%s'\n", fileName);
//        exit(1);
//   }
//
//   ppmFile ppm;
//   pixelsRGB rgb;
//
//   ppm.pSize = (char*)malloc(3);
//   ppm.description = (char*)malloc(10000);
//
//   // formatted read of header
//
//   //read image format
//   if (!fgets(ppm.pSize, sizeof(ppm.pSize), fr)) {
//        perror(fileName);
//        exit(1);
//   }
//
//   //check the image format
//   if (ppm.pSize[0] != 'P' || bufppm.pSize[1] != '6') {
//     fprintf(stderr, "Invalid image format (must be 'P6')\n");
//     exit(1);
//   }
//
//   //check for comments
//   int comment = getc(fr);
//   while (comment == '#') {
//   while (getc(fr) != '\n') ;
//        comment = getc(fr);
//   }
//
//   ungetc(comment, fr);
//
//   if (fscanf(fr, "%d %d", &ppm.width, &ppm.height) != 2) {
//     fprintf(stderr, "Invalid image size (error loading '%s')\n", fileName);
//     exit(1);
//   }
//
//   if (fscanf(fr, "%d", &ppm.maximum) != 1) {
//     fprintf(stderr, "Invalid rgb component (error loading '%s')\n", fileName);
//     exit(1);
//   }
//
//   //check rgb component depth
//   if (ppm.maximum!= RGB_COMPONENT_COLOR) {
//        fprintf(stderr, "'%s' does not have 8-bits components\n", fileName);
//        exit(1);
//   }
//
//   ppm.rgb.r = matrixMalloc(ppm.width, ppm.height);
//   ppm.rgb.g = matrixMalloc(ppm.width, ppm.height);
//   ppm.rgb.b = matrixMalloc(ppm.width, ppm.height);
//
//   int colorPixel;
//   int l = 0;
//   int c = 0;
//
//   for (int i = 0; i < ppm.width * ppm.height; i++ )
//   {
//     if ( c == ppm.width ) {
//       c = 0;
//       l++;
//     }
//
//     fscanf(fr, "%d\n", &colorPixel);
//
//     if ( i % 3 == 0 ) ppm.rgb.r[l][c] = colorPixel;
//     if ( i % 3 == 1 ) ppm.rgb.g[l][c] = colorPixel;
//     if ( i % 3 == 2 ) ppm.rgb.b[l][c] = colorPixel;
//
//     c++;
//   }
//
//   fclose(fr);
//   return ppm;
// }

pixelsYUV convertFromRGBtoYUV(int width, int height, pixelsRGB rgb) {
  pixelsYUV yuv;

  yuv.y = matrixMalloc(width, height);
  yuv.u = matrixMalloc(width, height);
  yuv.v = matrixMalloc(width, height);

  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
    {
      //1
      // yuv.y[i][j] = 0.299 * rgb.r[i][j] + 0.587 * rgb.g[i][j] + 0.114 * rgb.b[i][j];
      // yuv.u[i][j] = 128 - 0.1687 * rgb.r[i][j] - 0.3312 * rgb.g[i][j] + 0.5 * rgb.b[i][j];
      // yuv.v[i][j] = 128 + 0.5 * rgb.r[i][j] - 0.4186 * rgb.g[i][j] - 0.0813 * rgb.b[i][j];


      // yuv.y[i][j] = (0.299 * rgb.r[i][j]) + (0.587 * rgb.g[i][j]) + (0.114 * rgb.b[i][j]);
      // yuv.u[i][j] = -(0.147 * rgb.r[i][j]) - (0.289 * rgb.g[i][j]) + (0.436 * rgb.b[i][j]);
      // yuv.v[i][j] = (0.615 * rgb.r[i][j]) - (0.515 * rgb.g[i][j]) - (0.100 * rgb.b[i][j]);


      //2
      yuv.y[i][j] = 0.257 * rgb.r[i][j] + 0.504 * rgb.g[i][j] + 0.098 * rgb.b[i][j] + 16;
      yuv.u[i][j] = 0.439 * rgb.r[i][j] - 0.368 * rgb.g[i][j] + 0.071 * rgb.b[i][j] + 128;
      yuv.v[i][j] = 0.148 * rgb.r[i][j] - 0.291 * rgb.g[i][j] - 0.439 * rgb.b[i][j] + 128;
    }

    return yuv;
}

int clampTo8Bit(int v) {
  if ( (v & ~0xFF) != 0 )
    v = ((~v) >> 31) & 0xFF;
  return v;
}


pixelsRGB convertFromYUVtoRGB(int width, int height, pixelsYUV yuv) {
  pixelsRGB rgb;

  rgb.r = matrixMalloc(width, height);
  rgb.g = matrixMalloc(width, height);
  rgb.b = matrixMalloc(width, height);

  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
    {
      //2
      rgb.r[i][j] = 1.164 * (yuv.y[i][j] - 16) + 1.596 * (yuv.u[i][j] - 128);
      rgb.g[i][j] = 1.164 * (yuv.y[i][j] - 16) - 0.813 * (yuv.u[i][j] - 128) - 0.392 * (yuv.v[i][j] - 128);
      rgb.b[i][j] = 1.164 * (yuv.y[i][j] - 16) + 2.017 * (yuv.v[i][j] - 128);

      rgb.r[i][j] = clampTo8Bit(rgb.r[i][j]);
      rgb.g[i][j] = clampTo8Bit(rgb.g[i][j]);
      rgb.b[i][j] = clampTo8Bit(rgb.b[i][j]);
      //1
      // rgb.r[i][j] = yuv.y[i][j]  + 1.140 * yuv.v[i][j];
      // rgb.g[i][j] = yuv.y[i][j]  - (0.394 * yuv.u[i][j]) - (0.581 * yuv.v[i][j]);
      // rgb.b[i][j] = yuv.y[i][j]  + 2.032 * yuv.u[i][j];
    }

    return rgb;
}

// void writeInFile(const char* fileName, char* pSize,  char* description, int width, int height, int maximum, pixelsRGB rgb) {
//   FILE* fr = fopen(fileName, "w");
//
//   fprintf(fr, "%s\n%s\n%d %d\n%d\n", pSize, description, width, height, maximum);
//
//   for (int i = 0; i < height; i++)
//     for(int j = 0; j < width; j++)
//       fprintf(fr, "%d\n%d\n%d\n", rgb.r[i][j], rgb.g[i][j], rgb.b[i][j]);
//
//   fclose(fr);
// }


PPMImage *readPPM(const char *filename)
{
  char buff[16];
  PPMImage *img;
  FILE *fp;
  int c, rgb_comp_color;
  //open PPM file for reading
  fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Unable to open file '%s'\n", filename);
    exit(1);
  }

  //read image format
  if (!fgets(buff, sizeof(buff), fp)) {
    perror(filename);
    exit(1);
  }

  //check the image format
  if (buff[0] != 'P' || buff[1] != '3') {
    fprintf(stderr, "Invalid image format (must be 'P6')\n");
    exit(1);
  }

  //alloc memory form image
  img = (PPMImage *)malloc(sizeof(PPMImage));
  if (!img) {
    fprintf(stderr, "Unable to allocate memory\n");
    exit(1);
  }

  //check for comments
  c = getc(fp);
  while (c == '#') {
    while (getc(fp) != '\n') ;
      c = getc(fp);
  }
  ungetc(c, fp);

  //read image size information
  if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
    fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
    exit(1);
  }

  //read rgb component
  if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
    fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
    exit(1);
  }

  //check rgb component depth
  if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
    fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
    exit(1);
  }

  while (fgetc(fp) != '\n') ;

  //memory allocation for pixel data
  img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));

  if (!img) {
    fprintf(stderr, "Unable to allocate memory\n");
    exit(1);
  }

  //read pixel data from file
  if (fread(img->data, 3 * img->x, img->y, fp) != img->y) {
    fprintf(stderr, "Error loading image '%s'\n", filename);
    exit(1);
  }

  fclose(fp);
  return img;
}
void writePPM(const char *filename, PPMImage *img)
{
  FILE *fp;
  //open file for output
  fp = fopen(filename, "w");
  if (!fp) {
    fprintf(stderr, "Unable to open file '%s'\n", filename);
    exit(1);
  }

  //write the header file
  //image format
  fprintf(fp, "P3\n");

  //comments
  fprintf(fp, "# Created by %s\n",CREATOR);

  //image size
  fprintf(fp, "%d %d\n",img->x,img->y);

  // rgb component depth
  fprintf(fp, "%d\n", RGB_COMPONENT_COLOR);

  // pixel data
  fwrite(img->data, 3 * img->x, img->y, fp);
  fclose(fp);
}

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("Invalid number of arguments!\n");
    return 1;
  }

  PPMImage *image;
  image = readPPM(argv[1]);
  writePPM("final.ppm",image);

  // ppmFile ppm;
  // pixelsYUV yuv_encoder;
  //
  // // encoder pat
  // ppm = readFromFile(argv[1]);
  // writeInFile("final.ppm", ppm.pSize, ppm.description, ppm.width, ppm.height, ppm.maximum, ppm.rgb);
  //
  // yuv_encoder = convertFromRGBtoYUV(ppm.width, ppm.height, ppm.rgb);
  //
  // // decoder part
  // pixelsRGB rgb_decoder;
  // rgb_decoder = convertFromYUVtoRGB(ppm.width, ppm.height, yuv_encoder);
  return 0;
}
