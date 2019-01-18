#include "sepia_filter.h"

int main(){
    FILE *img_file = fopen("resources/big_asm.bmp", "r+b");
    image source_img;
    read_from_bmp(img_file, &source_img);
    sepia_filter_sse(&source_img);
    fclose(img_file);
    return 0;
}
