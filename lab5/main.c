#include "bmp_header.h"

 read_status read_from_bmp(FILE *in, image *read_in){
  uint32_t width;
  uint32_t height;

  //check type of file
  uint16_t type;
  fread(&type, 1, 2, in);
  if(type != BMP_CODE){
    return READ_INVALID_SIGNATURE;
  }

  //read file size
  uint32_t file_size;
  fread(&file_size, 4, 1, in);

  //reading width and height
  fseek(in, 12, SEEK_CUR);
  fread(&width, 4, 1, in);
  fread(&height, 4, 1, in);

  //padding width
  width += (4 - width % 4);

  //check header
  if((width * height * sizeof(pixel) + BMP_OFFSET) != file_size){
    return READ_INVALID_HEADER;
  }

  //set width and height
  read_in->width = width;
  read_in->height = height;

  //reading pixels
  uint32_t i;
  fseek(in, BMP_OFFSET, SEEK_SET);
  read_in->data = (pixel*) malloc(width * height * sizeof(pixel));
  for(i = 0; i < height * width; i++){
    pixel *px = read_in->data + i;
    fread(px, 1, sizeof(pixel), in);
    }
  return READ_OK;
}

write_status write_to_bmp(FILE *out, image *img){
  BMPHeader *header = (BMPHeader*) malloc(sizeof(BMPHeader));

  //writing bmp code
  header->type = BMP_CODE;
  fwrite(&header->type, 2, 1, out);

  //writing file size
  header->file_size = img->height * img->width * sizeof(pixel) + BMP_OFFSET;
  fwrite(&header->file_size, 4, 1, out);

  //writing reserved bytes
  header->reserved = 0;
  fwrite(&header->reserved, 4, 1, out);

  //writing offset
  header->offset = BMP_OFFSET;
  fwrite(&header->offset, 4, 1, out);

  header->dib_header_size = 40;
  fwrite(&header->dib_header_size, 4, 1, out);

  fwrite(&img->width, 4, 1, out);
  fwrite(&img->height, 4, 1, out);

  header->planes = 1;
  fwrite(&header->planes, 2, 1, out);

  header->bits_pixel = 24;
  fwrite(&header->bits_pixel, 2, 1, out);

  header->compression = 0;
  header->image_size_bytes = img->height * img->width * sizeof(pixel);
  header->x_ppm = 0xb12;
  header->y_ppm = 0xb12;
  header->num_colors = 0;
  header->important_colors = 0;
  fwrite(&header->compression, 4, 6, out);

  //writing pixels
  uint64_t i;
  for(i = 0; i < img->height * img->width; i++){
    pixel px = *(img->data + i);
    fwrite(&px, 1, 3, out);
  }
  return WRITE_OK;

}

image* rotate(image *source_image){
  image* new_image = (image*) malloc(sizeof(image));
  new_image->width = source_image->height;
  new_image->height = source_image->width;
  new_image->data = (pixel*) malloc(new_image->width * new_image->height * 3);
  uint32_t row;
  uint32_t column;
  for(row = 0; row < new_image->height; row++){
    for (column = 0; column < new_image->width; column++) {
      pixel *px = source_image->data + source_image->width*(column + 1) - row;
      pixel *new_px = new_image->data + column + row * new_image->width;
      new_px->blue = px->blue;
      new_px->green = px->green;
      new_px->red = px->red;
    }
  }
  return new_image;
}

int main(){
  FILE *penguin = fopen("../resources/penguin.bmp", "rb");
  image *img = (image*) malloc(sizeof(image));
  read_from_bmp(penguin, img);
  fclose(penguin);
  image* new_one = rotate(img);

  FILE *new_penguin = fopen("../resources/new_penguin.bmp", "wb");
  write_to_bmp(new_penguin, new_one);
  fclose(new_penguin);
  return 0;
}
