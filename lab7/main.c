#include "sepia_filter.h"

int main(int argc, char *argv[]){
    struct option long_options[] = {
            {"file",     required_argument, NULL, 0},
            {"method", optional_argument, NULL, 1},
            {NULL, 0,                       NULL, 0}
    };
    char *method;
    char *file_name;
    int option_index;

    while (1) {
            int flag = getopt_long_only(argc, argv, "", long_options, &option_index);
            if (flag == -1)
                    break;

            switch (flag) {
            case 0:
                    file_name = optarg;
                    break;
            case 1:
                    method = optarg;
                    break;
            default:
                    printf("%s\n", "Wrong arguments");
            }
    }

    FILE *file_image = fopen(file_name, "r+b");
    if(file_image == NULL){
      puts("File did not find");
      exit(1);
    }

    image source_img;
    read_status rd_status = read_from_bmp(file_image, &source_img);

    if (rd_status == READ_INVALID_SIGNATURE) {
            puts("Invalid file's signature");
            exit(1);
    } else if (rd_status == READ_INVALID_HEADER) {
            puts("Invalid bmp header");
            exit(1);
    }

    if(strcmp(method, "asm") == 0){
      sepia_filter_sse(&source_img);
    }
    else{
      sepia_filter_c(&source_img);
    }
    fclose(file_image);
    return 0;
}
