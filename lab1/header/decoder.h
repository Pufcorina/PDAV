#include "ppm.h"
#define CREATOR "Cwori"
#define RGB_COMPONENT_COLOR 255

void writePPM(const char *filename, struct PPMImage *img);
void writePPM_P6(const char *filename, struct PPMImage *img);
void writePPM_P3(const char *filename, struct PPMImage *img);

void decode_ppm(struct BlocksImg* block, const char* filename);
struct PPMImage* convertMatrixesToArray(struct Pixels* img);
