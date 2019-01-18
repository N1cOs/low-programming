#include "sepia_filter.h"

extern void sepia_chunk(pixel* data);

int main(){
  sepia_chunk(NULL);
    // FILE *img_file = fopen("resources/3_3.bmp", "r+b");
    // image source_img;
    // read_from_bmp(img_file, &source_img);
    // sepia_filter_c(&source_img);
    // fclose(img_file);
    // image test_image;
    // test_image.height = 2;
    // test_image.width = 4;
    // uint8_t padding = (4 - test_image.width * sizeof(pixel) % 4) % 4;
    // test_image.data = malloc((test_image.width + padding) * test_image.height * sizeof(pixel));
    // pixel *data = test_image.data;
    //
    // for(int j = 0; j < test_image.height; j++){
    //   for(int i = 0; i < test_image.width; i++){
    //     pixel *px = (void*)(data + j * test_image.width + i) + j * padding;
    //     px->blue = 80 + 2 * i + j;
    //     px->green = 110 + 3 * i + j;
    //     px->red = 160 + 4 * i + j;
    //   }
    //   for(int i = 0; i < padding; i++){
    //     uint8_t *p = ((uint8_t*)(data + (j + 1) * test_image.width + i) + j * padding);
    //     *p = 0;
    //   }
    // }
    // sepia_filter_sse(&test_image);


    return 0;
}
