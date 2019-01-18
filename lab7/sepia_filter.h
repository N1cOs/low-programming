#include "bmp.h"
#include <errno.h>

#ifndef SEPIA_FILTER
#define SEPIA_FILTER

void sepia_filter_c(image *source_image);
void sepia_filter_sse(image *source_image);

#endif
