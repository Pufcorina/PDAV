#include "ppm.h"
#include "matrixMan.h"

#define CREATOR "Cwori"
#define RGB_COMPONENT_COLOR 255

struct PPMImage* readPPM_P6(const char* filename);
struct PPMImage* readPPM_P3(const char* filename);
struct PPMImage* readPPM(const char* filename);

struct BlocksImg* encode_ppm(const char* filename);
struct Pixels* convertArrayToMatrixes(struct PPMImage* img);
