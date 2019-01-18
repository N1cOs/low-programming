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

static void sepia_chunk(pixel* data){
  const float coeffs[3][12] = {
          {.131f, .168f, .189f, .131f, .168f, .189f, .131f, .168f, .189f, .131f, .168f, .189f},
          {.543f, .686f, .769f, .543f, .686f, .769f, .543f, .686f, .769f, .543f, .686f, .769f},
          {.272f, .349f, .393f, .272f, .349f, .393f, .272f, .349f, .393f, .272f, .349f, .393f}
  };
  uint8_t results[12];

  int temp = 3;
  for(int i = 0; i < 3; i++){
    pixel *px_1 = data + i;
    pixel *px_2 = data + i + 1;

    float blue[4];
    float green[4];
    float red[4];
    float result[4];

    for(int j = 0; j < temp; j++){
      blue[j] = px_1->blue;
      green[j] = px_1->green;
      red[j] = px_1->red;
    }

    for(int j = temp; j < 4; j++){
      blue[j] = px_2->blue;
      green[j] = px_2->green;
      red[j] = px_2->red;
    }

    float coeffs_1[4];
    float coeffs_2[4];
    float coeffs_3[4];

    for(int j = 0; j < 4; j++){
      coeffs_1[j] = coeffs[0][j + 4 * i];
      coeffs_2[j] = coeffs[1][j + 4 * i];
      coeffs_3[j] = coeffs[2][j + 4 * i];
    }

    set_coeffs(coeffs_1, coeffs_2, coeffs_3);
    sepia_one_sse(blue, green, red, result);

    for(int j = 0; j < 4; j++){
      results[4 * i + j] = sat(result[j]);
    }
    temp--;
  }
  memcpy(data, results, 12);
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
