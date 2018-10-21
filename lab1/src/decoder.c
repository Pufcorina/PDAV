#include "../header/decoder.h"
#include "../header/matrixMan.h"
#include<stdio.h>
#include<stdlib.h>


void writePPM_P6(const char *filename, struct PPMImage *img) {
  FILE *fp;
  //open file for output
  fp = fopen(filename, "wb");
  if (!fp) {
       fprintf(stderr, "Unable to open file '%s'\n", filename);
       exit(1);
  }

  //write the header file
  //image format
  fprintf(fp, "P6\n");

  //comments
  fprintf(fp, "# Created by %s\n",CREATOR);

  //image size
  fprintf(fp, "%d %d\n",img->x,img->y);

  // rgb component depth
  fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

  // pixel data
  fwrite(img->data, 3 * img->x, img->y, fp);
  fclose(fp);
}

void writePPM_P3(const char *filename, struct PPMImage *img) {
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
  fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

  for (int i = 0; i < img->y * img->x; i++) {
      fprintf(fp,"%d\n%d\n%d\n", img->data[i].red, img->data[i].green, img->data[i].blue);
  }

  fclose(fp);
}

void writePPM(const char* filename, struct PPMImage* img) {
  if ( img->format == '3' )
    writePPM_P3(filename, img);
  else
    writePPM_P6(filename, img);
}


struct PPMImage* convertMatrixesToArray(struct Pixels* img) {
  struct PPMImage* ppm = (struct PPMImage*)malloc(sizeof(struct PPMImage));

  ppm->x = img->width;
  ppm->y = img->height;
  ppm->format = img->format;

  ppm->data = (struct PPMPixel*)malloc(sizeof(struct PPMPixel) * img->width * img->height);

  int k = 0;
  for (int i = 0; i < img->height; i++)
    for (int j = 0; j < img->width; j++)
    {
      struct PPMPixel data;
      data.red = img->a[i][j];
      data.green = img->b[i][j];
      data.blue = img->c[i][j];
      ppm->data[k] = data;
      k++;
    }

  return ppm;
}

struct Pixels* convertFromYUVtoRGB(struct Pixels* yuv) {
  struct Pixels* rgb = (struct Pixels*)malloc(sizeof(struct Pixels));
  rgb->width = yuv->width;
  rgb->height = yuv->height;
  rgb->format = yuv->format;

  int width = yuv->width;
  int height = yuv->height;


  rgb->a = matrixMalloc(width, height);
  rgb->b = matrixMalloc(width, height);
  rgb->c = matrixMalloc(width, height);

  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
    {
      //1

      // rgb->a[i][j] = yuv->a[i][j]  + 1.140 * yuv->c[i][j];
      // rgb->b[i][j] = yuv->a[i][j]  - (0.394 * yuv->b[i][j]) - (0.581 * yuv->c[i][j]);
      // rgb->c[i][j] = yuv->a[i][j]  + 2.032 * yuv->b[i][j];

      //2

      rgb->a[i][j] = 1.164 * (yuv->a[i][j] - 16) + 1.596 * (yuv->b[i][j] - 128);
      rgb->b[i][j] = 1.164 * (yuv->a[i][j] - 16) - 0.813 * (yuv->b[i][j] - 128) - 0.392 * (yuv->c[i][j] - 128);
      rgb->c[i][j] = 1.164 * (yuv->a[i][j] - 16) + 2.017 * (yuv->c[i][j] - 128);

      rgb->a[i][j] = clampTo8Bit(rgb->a[i][j]);
      rgb->b[i][j] = clampTo8Bit(rgb->b[i][j]);
      rgb->c[i][j] = clampTo8Bit(rgb->c[i][j]);
    }

  return rgb;
}

void printMatrix(const char* filename, struct EncodedMatrix* matrix, int length) {
  FILE *fp;
  //open file for output
  fp = fopen(filename, "w");
  if (!fp) {
       fprintf(stderr, "Unable to open file '%s'\n", filename);
       exit(1);
  }

  for (size_t i = 0; i < length; i++) {
    for (size_t j = 0; j < matrix[i].size; j++) {
      for (size_t k = 0; k < matrix[i].size; k++) {
        fprintf(fp, "%d ", matrix[i].matrix[j][k]);
      }
      fprintf(fp, "\n");
    }

    fprintf(fp, "\n\n");
  }

  fclose(fp);
}



void decode_ppm(struct BlocksImg* block) {
  int length = block->width * block->height / 64;
  printMatrix("./output/yBlock", block->a, length);
  printMatrix("./output/uBlock", block->b, length * 4);
  printMatrix("./output/vBlock", block->c, length * 4);


  // pi = convertFromYUVtoRGB(pi);
  // struct PPMImage* img = convertMatrixesToArray(pi);
  //
  // writePPM("./output/finalFile.ppm", img);
}