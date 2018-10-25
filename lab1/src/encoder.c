#include "../header/encoder.h"
#include "../header/matrixMan.h"
#include<stdio.h>
#include<stdlib.h>

struct PPMImage* readPPM_P6(const char* filename) {
  char buff[16];
  struct PPMImage *img;
  FILE *fp;
  int c, rgb_comp_color;
  //open PPM file for reading
  fp = fopen(filename, "rb");

  //read image format
  if (!fgets(buff, sizeof(buff), fp)) {
    perror(filename);
    exit(1);
  }

  //alloc memory form image
  img = (struct PPMImage *)malloc(sizeof(struct PPMImage));
  if (!img) {
    fprintf(stderr, "Unable to allocate memory\n");
    exit(1);
  }

  img->format = '6';

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
  img->data = (struct PPMPixel*)malloc(img->x * img->y * sizeof(struct PPMPixel));

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

struct PPMImage* readPPM_P3(const char* filename) {
  char buff[16];
  struct PPMImage *img;
  FILE *fp;
  int c, rgb_comp_color;
  //open PPM file for reading
  fp = fopen(filename, "r");

  //read image format
  if (!fgets(buff, sizeof(buff), fp)) {
    perror(filename);
    exit(1);
  }

  //alloc memory form image
  img = (struct PPMImage *)malloc(sizeof(struct PPMImage));
  if (!img) {
    fprintf(stderr, "Unable to allocate memory\n");
    exit(1);
  }

  img->format = '3';

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
  img->data = (struct PPMPixel*)malloc(img->x * img->y * sizeof(struct PPMPixel));

  if (!img) {
    fprintf(stderr, "Unable to allocate memory\n");
    exit(1);
  }

  //read pixel data from file
  for( int i = 0; i < img->x * img->y; i++)
    fscanf(fp, "%d\n%d\n%d\n", &img->data[i].red, &img->data[i].green, &img->data[i].blue);

  fclose(fp);
  return img;
}

struct PPMImage* readPPM(const char* filename) {
  FILE *fp;
  fp = fopen(filename, "rb");
  if (!fp) {
       fprintf(stderr, "Unable to open file '%s'\n", filename);
       exit(1);
  }

  char buff[16];

  //read image format
  if (!fgets(buff, sizeof(buff), fp)) {
       perror(filename);
       exit(1);
  }

  //check the image format
  if (buff[0] != 'P' && buff[1] != '6' && buff[1] != '3') {
       fprintf(stderr, "Invalid image format (must be 'P6' or 'P3')\n");
       exit(1);
  }

  fclose(fp);

  if ( buff[1] == '3' )
    return readPPM_P3(filename);
  return readPPM_P6(filename);
}

struct Pixels* convertFromRGBtoYUV(struct Pixels* rgb) {
  struct Pixels* yuv = (struct Pixels*)malloc(sizeof(struct Pixels));
  yuv->width = rgb->width;
  yuv->height = rgb->height;
  yuv->format = rgb->format;

  int width = rgb->width;
  int height = rgb->height;


  yuv->a = matrixMalloc(width, height);
  yuv->b = matrixMalloc(width, height);
  yuv->c = matrixMalloc(width, height);

  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
    {
      int r = rgb->a[i][j];
      int g = rgb->b[i][j];
      int b = rgb->c[i][j];

      // yuv->a[i][j] = ((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
      // yuv->b[i][j] = ((-33 * r - 74 * g + 112 * b + 128) >> 8) + 128;
      // yuv->c[i][j] = ((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;

      yuv->a[i][j] = 0.299 * rgb->a[i][j] + 0.587 * rgb->b[i][j] + 0.114 * rgb->c[i][j];
      yuv->b[i][j] = -0.169 * rgb->a[i][j] - 0.331 * rgb->b[i][j] + 0.499 * rgb->c[i][j] + 128;
      yuv->c[i][j] = 0.449 * rgb->a[i][j] - 0.418 * rgb->b[i][j] - 0.0813 * rgb->c[i][j] + 128;
    }

  return yuv;
}


struct Pixels* convertArrayToMatrixes(struct PPMImage* img) {
  struct Pixels* pi = (struct Pixels*)malloc(sizeof(struct Pixels));
  pi->width = img->x;
  pi->height = img->y;
  pi->format = img->format;

  int width = pi->width;
  int height = pi->height;


  pi->a = matrixMalloc(width, height);
  pi->b = matrixMalloc(width, height);
  pi->c = matrixMalloc(width, height);

  int i = 0;
  int j = 0;


  for (int k = 0; k < img->x * img->y; k++)
  {
    if (j == pi->width ) {
      j = 0;
      i++;
    }
    pi->a[i][j] = img->data[k].red;
    pi->b[i][j] = img->data[k].green;
    pi->c[i][j] = img->data[k].blue;
    j++;
  }

  return pi;
}

int averageBlock(int poz_i, int poz_j, unsigned int** matrix) {
  int sum = 0;
  for (int i = poz_i; i < 2 + poz_i; i++)
    for(int j = poz_j; j < 2 + poz_j; j++)
    {
      sum += matrix[i][j];
    }
  sum /= 2;
  return sum;
}

struct EncodedMatrix submatrice4Blocks(int poz_i, int poz_j, int sizeBlock, unsigned int** matrix) {
  struct EncodedMatrix submat;

  submat.i = poz_i;
  submat.j = poz_j;
  submat.size = 4;
  submat.matrix = matrixMalloc(4, 4);

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
    {
      submat.matrix[i][j] = averageBlock(i * 2 + poz_i, j * 2 + poz_j, matrix);
    }

  return submat;
}

struct EncodedMatrix submatrice(int poz_i, int poz_j, int sizeBlock, unsigned int** matrix) {
  struct EncodedMatrix submat;

  submat.i = poz_i;
  submat.j = poz_j;
  submat.size = sizeBlock;
  submat.matrix = matrixMalloc(sizeBlock, sizeBlock);

  for(int i = poz_i; i < poz_i + sizeBlock; i++)
    for(int j = poz_j; j < poz_j + sizeBlock; j++)
      submat.matrix[i - poz_i][j - poz_j] = matrix[i][j];

  return submat;
}

struct EncodedMatrix* splitMatrix(unsigned int** matrix, int sizeBlock, int width, int height, struct EncodedMatrix (*f)(int poz_i, int poz_j, int sizeBlock, unsigned int** matrix)) {
  int length = (width * height) / (sizeBlock * sizeBlock);
  struct EncodedMatrix* blocks = (struct EncodedMatrix*)malloc(sizeof(struct EncodedMatrix) * length);

  int k = 0;
  for (int i = 0; i < height; i += sizeBlock)
    for(int j = 0; j < width; j += sizeBlock) {
        blocks[k] = f(i, j, sizeBlock, matrix);
        k++;
    }
  return blocks;
}

struct BlocksImg* splitImageInBlocks(struct Pixels* img) {
  struct BlocksImg* blockImg = (struct BlocksImg*)malloc(sizeof(struct BlocksImg));
  blockImg->width = img->width;
  blockImg->height = img->height;
  blockImg->format = img->format;


  blockImg->a = splitMatrix(img->a, 8, img->width, img->height, submatrice);
  blockImg->b = splitMatrix(img->b, 8, img->width, img->height, submatrice4Blocks);
  blockImg->c = splitMatrix(img->c, 8, img->width, img->height, submatrice4Blocks);

  return blockImg;
}

struct BlocksImg* encode_ppm(const char* filename) {
  struct PPMImage *image;
  struct Pixels *img, *yuv;
  struct BlocksImg* blockImg;

  image = readPPM(filename);
  img = convertArrayToMatrixes(image);
  yuv = convertFromRGBtoYUV(img);

  blockImg = splitImageInBlocks(yuv);
  return blockImg;
}
