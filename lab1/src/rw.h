#include "ppm.h"

#define CREATOR "Cwori"
#define RGB_COMPONENT_COLOR 255

PPMImage* readPPM_P6(const char* filename);
PPMImage* readPPM_P3(const char* filename);
PPMImage* readPPM(const char* filename);
void writePPM(const char *filename, PPMImage *img);
void writePPM_P6(const char *filename, PPMImage *img);
void writePPM_P3(const char *filename, PPMImage *img);
