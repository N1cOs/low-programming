#include "bmp.h"

bmp_header *get_bmp_header(image *img) {
        bmp_header *header = (bmp_header *) malloc(sizeof(bmp_header));
        uint8_t padding = (4 - img->width * sizeof(pixel) % 4) % 4;

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

read_status read_from_bmp(FILE *in, image *read_in) {
        uint32_t width;
        uint32_t height;

        //check type of file
        uint16_t type;
        fread(&type, 2, 1, in);
        if (type != BMP_CODE) {
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
        uint8_t padding_width = (4 - width * sizeof(pixel) % 4) % 4;

        int file_descriptor = fileno(in);
        read_in->data = mmap(NULL, (width * sizeof(pixel) + padding_width) * height, PROT_READ | PROT_WRITE,
                             MAP_SHARED, file_descriptor, 0);
        read_in->data = (pixel *) ((void *) read_in->data + BMP_OFFSET);
        return READ_OK;
}

write_status write_to_bmp(FILE *out, image *img) {
        fseek(out, 0, SEEK_SET);
        bmp_header *header = get_bmp_header(img);
        fwrite(header, sizeof(bmp_header), 1, out);

        uint8_t padding_width = (4 - img->width * sizeof(pixel) % 4) % 4;
        uint64_t i;
        uint32_t zero_padding = 0;
        for (i = 0; i < img->height * (img->width + 1); i++) {
                pixel *px = img->data + i;
                px = (pixel *) (((uint8_t *) px) - zero_padding * (3 - padding_width));
                if ((i + 1) % (img->width + 1) == 0) {
                        fwrite(px, 1, padding_width, out);
                        zero_padding++;
                } else {
                        fwrite(px, 1, 3, out);
                }
        }
        return WRITE_OK;
}

write_status write_mmap_to_bmp(image *source_image, image *dest_img) {
        bmp_header *header = get_bmp_header(dest_img);

        uint8_t padding_width = (4 - dest_img->width * sizeof(pixel) % 4) % 4;
        memcpy((void *) source_image->data - BMP_OFFSET, header, sizeof(bmp_header));
        memcpy(source_image->data, dest_img->data, (dest_img->width * sizeof(pixel) + padding_width) * dest_img->height);
        return WRITE_OK;
}

image *rotate(image *source_image) {
        image *new_image = malloc(sizeof(image));
        new_image->height = source_image->width;
        new_image->width = source_image->height;

        uint8_t old_padding = (4 - source_image->width * sizeof(pixel) % 4) % 4;
        uint8_t new_padding = (4 - new_image->width * sizeof(pixel) % 4) % 4;
        new_image->data = mmap(NULL, (new_image->width * sizeof(pixel) + new_padding) * new_image->height,
                               PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);

        uint32_t row;
        uint32_t column;
        for (row = 0; row < new_image->height; row++) {
                pixel source_px[new_image->width];
                for (column = 0; column < new_image->width; column++) {
                        pixel *px =
                                (void *) (source_image->data + source_image->width * (column + 1) - row - 1) + old_padding * column;
                        source_px[column] = *px;
                }
                void *dest = (void *) new_image->data + (new_image->width * sizeof(pixel) + new_padding) * row;
                memcpy(dest, source_px, new_image->width * sizeof(pixel));
        }
        //munmap(source_image->data, (source_image->width + old_padding) * source_image->height);
        return new_image;
}

int main(int argc, char *argv[]) {

        struct option long_options[] = {
                {"file",     required_argument, NULL, 0},
                {"rotation", optional_argument, NULL, 1},
                {NULL, 0,                       NULL, 0}
        };
        int option_index;
        char *file_name;
        int angle = 90;

        while (1) {
                int flag = getopt_long_only(argc, argv, "", long_options, &option_index);
                if (flag == -1)
                        break;

                switch (flag) {
                case 0:
                        file_name = optarg;
                        break;
                case 1:
                        angle = atoi(optarg);
                        break;
                default:
                        printf("%s\n", "Wrong arguments");
                }
        }

        FILE *file_image = fopen(file_name, "r+b");
        image source_img;
        read_status rd_status = read_from_bmp(file_image, &source_img);

        if (rd_status == READ_INVALID_SIGNATURE) {
                puts("Invalid file's signature");
                exit(1);
        } else if (rd_status == READ_INVALID_HEADER) {
                puts("Invalid bmp header");
                exit(1);
        }

        image *new_image = &source_img;
        uint16_t i;
        for (i = 0; i < ((angle / 90) % 4); i++) {
                new_image = rotate(new_image);
        }
        write_mmap_to_bmp(&source_img, new_image);
        fclose(file_image);
        return 0;
}
