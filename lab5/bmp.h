#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#define BMP_CODE 0x4d42
#define BMP_OFFSET 54

typedef struct __attribute__((packed)){
  uint16_t type;
  uint32_t file_size;
  uint32_t reserved;
  uint32_t offset;
  uint32_t dib_header_size;

  uint32_t width;
  uint32_t height;
  uint16_t planes;
  uint16_t bits_pixel;
  uint32_t compression;
  uint32_t image_size_bytes;
  uint32_t x_ppm;
  uint32_t y_ppm;
  uint32_t num_colors;
  uint32_t important_colors;
} BMPHeader;

typedef struct{
  uint8_t blue, green, red;
} pixel;

typedef struct{
  uint32_t width, height;
  pixel *data;
} image;

typedef enum read_status{
  READ_OK = 0,
  READ_INVALID_SIGNATURE,
  READ_INVALID_HEADER
} read_status;

typedef enum write_status{
  WRITE_OK = 0,
  WRITE_ERROR
} write_status;

read_status read_from_bmp(FILE *in, image *read_in);

image* rotate(image *source_image);

write_status write_to_bmp(FILE *out, image *img);
