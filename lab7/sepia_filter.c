#include "sepia_filter.h"

void sepia_one_sse(float blue[4], float green[4], float red[4], float out[4]);
void set_coeffs(const float c1[4], const float c2[4], const float c3[4]);

static uint8_t sat( uint64_t x) {
        if (x < 256)
                return x;
        return 255;
}

static void sepia_one(pixel* px ) {
        static const float c[3][3] =  {
                { .393f, .769f, .189f },
                { .349f, .686f, .168f },
                { .272f, .543f, .131f }
        };
        pixel old = *px;
        px->red = sat(old.red * c[0][0] + old.green * c[0][1] + old.blue * c[0][2]);
        px->green = sat(old.red * c[1][0] + old.green * c[1][1] + old.blue  * c[1][2]);
        px->blue = sat(old.red * c[2][0] + old.green * c[2][1] + old.blue * c[2][2]);
}

void sepia_chunk(pixel* data){
  const float coeffs[3][4] = {
          {.393f, .349f, .272f, .393f},
          {.769f, .686f, .543f, .769f},
          {.189f, .168f, .131f, .272f}
  };
  set_coeffs(coeffs[0], coeffs[1], coeffs[2]);

}

void sepia_filter_sse(image *img){
        const uint8_t padding_width = (4 - img->width * sizeof(pixel) % 4) % 4;
        const uint8_t manual_pixels = img->width % 4;
        const uint32_t sse_width = img->width - img->width % 4;

        for(int i = 0; i < img->height; i++) {
                for(int j = 0; j < sse_width; j += 4) {
                      sepia_chunk((void*)(img->data + i * img->width + j) + i * padding_width);
                }

                for(int j = 0; j < manual_pixels; j++){
                  sepia_one((void*)(img->data + sse_width * (i + 1) + manual_pixels * i + j) + i * padding_width);
                }
        }
}

void sepia_filter_c(image *img){
        const uint8_t padding_width = (4 - img->width * sizeof(pixel) % 4) % 4;
        for(int i = 0; i < img->height; i++) {
                for(int j = 0; j < img->width; j++) {
                        sepia_one((void*)(img->data + i * img->width + j) + i * padding_width);
                }
        }
}
