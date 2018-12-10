#include "bmp.h"

read_status read_from_bmp(FILE *in, image *read_in){
 uint32_t width;
 uint32_t height;

 //check type of file
 uint16_t type;
 fread(&type, 2, 1, in);
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

 //set width and height
 read_in->width = width;
 read_in->height = height;

 //width's length in bytes = width * sizeof(pixel) + padding_width
 uint8_t padding_width = (4 - width * sizeof(pixel) % 4);

 //check header
 if(((width * sizeof(pixel) + padding_width) * height  + BMP_OFFSET) != file_size){
   return READ_INVALID_HEADER;
 }

 //reading pixels
 uint32_t i;
 fseek(in, BMP_OFFSET, SEEK_SET);
 read_in->data = (pixel*) calloc(width * height, sizeof(pixel));
 uint32_t skiped_padding = 0;
 for(i = 0; i < height * (width + 1); i++){
   if((i + 1) % (width + 1) == 0){
     fseek(in, padding_width, SEEK_CUR);
     skiped_padding++;
   }
   else{
     pixel *px = read_in->data + i - skiped_padding;
     fread(px, sizeof(pixel), 1, in);
   }
 }
 return READ_OK;
}

BMPHeader * get_bmp_header(image *img){
  BMPHeader *header = (BMPHeader*) malloc(sizeof(BMPHeader));
  uint8_t padding = (4 - img->width * sizeof(pixel) % 4);

  //writing bmp code
  header->type = BMP_CODE;

  //writing file size
  header->file_size = (img->width * sizeof(pixel) + padding) * img->height + BMP_OFFSET;

  //writing reserved bytes
  header->reserved = 0;

  //writing offset
  header->offset = BMP_OFFSET;

  header->dib_header_size = 40;

  //writing width and height
  header->width = img->width;
  header->height = img->height;

  //writing planes
  header->planes = 1;

  //writing bits per pixel
  header->bits_pixel = 24;

  header->compression = 0;
  header->image_size_bytes = (img->width * sizeof(pixel) + padding) * img->height;
  header->x_ppm = 0xb12;
  header->y_ppm = 0xb12;
  header->num_colors = 0;
  header->important_colors = 0;
  return header;
}

write_status write_to_bmp(FILE *out, image *img){
  //writing header
  BMPHeader *header = get_bmp_header(img);
  fwrite(header, sizeof(BMPHeader), 1, out);

  uint8_t padding = (4 - img->width * sizeof(pixel) % 4);
  //writing pixels
  uint64_t i;
  uint32_t zero_padding = 0;
  for(i = 0; i < img->height * (img->width + 1); i++){
    pixel *px = img->data + i;
    px = (pixel*) (((uint8_t*) px) - zero_padding * (3 - padding));
    if((i + 1) % (img->width + 1) == 0){
      fwrite(px, 1, padding, out);
      zero_padding++;
    }
    else{
      fwrite(px, 1, 3, out);
    }
  }
  return WRITE_OK;
}

image* rotate(image *source_image){
  image* new_image = (image*) malloc(sizeof(image));
  new_image->width = source_image->height;
  new_image->height = source_image->width;

  uint8_t new_padding =  (4 - new_image->width * sizeof(pixel) % 4);
  new_image->data = (pixel*) malloc((new_image->width * sizeof(pixel) + new_padding) * new_image->height);

  uint32_t row;
  uint32_t column;
  for(row = 0; row < new_image->height; row++){
    for (column = 0; column < new_image->width; column++) {
      pixel *px = source_image->data + source_image->width * (column + 1) - row - 1;
      pixel *new_px = new_image->data + column + row * new_image->width;
      new_px = (pixel*) ((uint8_t*) new_px + row * new_padding);
      memcpy(new_px, px, sizeof(pixel));
    }
     uint8_t *zero_padding = ((uint8_t*)new_image->data) + new_image->width * sizeof(pixel) * (row + 1) + row * new_padding;
     uint8_t i;
     for(i = 0; i <= new_padding - 1; i++)
       *(zero_padding + i) = 0;
  }
  return new_image;
}

int main(int argc, char *argv[]){

  if(argc != 2){
    puts("Wrong number of arguments");
    exit(1);
  }

  FILE *file_image = fopen(argv[1], "rb");
  if(!file_image){
    puts("File does not exist");
    exit(1);
  }


  image *img = (image*) malloc(sizeof(image));
  read_status rd_status = read_from_bmp(file_image, img);
  fclose(file_image);
  if(rd_status == READ_INVALID_SIGNATURE){
    puts("Invalid file's signature");
    exit(1);
  }
  else if(rd_status == READ_INVALID_HEADER){
    puts("Invalid bmp header");
    exit(1);
  }

  image* new_one = rotate(img);
  FILE *new_img = fopen(argv[1], "wb");
  write_status wr_status = write_to_bmp(new_img, new_one);
  fclose(new_img);
  if(wr_status == WRITE_OK){
    puts("Your image successfully rotated");
  }
  return 0;
}
