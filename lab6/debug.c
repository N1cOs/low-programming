#include "mem.h"

void _malloc_debug_info( FILE *f, mem *address ) {
    size_t i;
    fprintf( f,"start: %p\nsize: %lu\nis_free: %d\n", (void*)address, address-> capacity, address-> is_free );
    for ( i = 0;i <  DEBUG_FIRST_BYTES  &&  i <  address-> capacity; i++)
        fprintf( f, "%hhX",((uint8_t *)address)[ sizeof( mem ) + i ] );
    fprintf(f, "\n\n");
}

void _malloc_debug_heap( FILE *f, mem *ptr ) {
    for( ; ptr; ptr = ptr->next )
        _malloc_debug_info( f, ptr );
}

int main(){
    FILE *debug_file = fopen("resources/test.txt", "w");
    heap_init(100);

    uint8_t *ptr = _malloc(5000);
    for(int i = 0; i < 5000; i++){
        *(ptr + i) = (i % 256);
    }
    uint8_t *ptr_2 = _malloc(10000);
    for(int i = 0; i < 10000; i++){
        *(ptr_2 + i) = (i % 255 + 1);
    }
    uint8_t *ptr_3 = _malloc(4000);
    for(int i = 0; i < 4000; i++){
        *(ptr_3 + i) = (i % 254 + 2);
    }

    _malloc_debug_heap(debug_file, (mem*)(ptr - sizeof(mem)));
    fprintf(debug_file, "\n%s\n\n", "after freeing memory");
    _free(ptr_2);

    uint8_t *ptr_4 = _malloc(20000);
    for(int i = 0; i < 4000; i++){
        *(ptr_4 + i) = (i % 253 + 3);
    }
    uint8_t *ptr_5 = _malloc(5000);
    for(int i = 0; i < 4000; i++){
        *(ptr_5 + i) = (i % 252 + 4);
    }
    _malloc(3000);

    _free(ptr_5);
    _free(ptr);
    _malloc_debug_heap(debug_file, (mem*)(ptr - sizeof(mem)));
    fclose(debug_file);
//    int value = 0;
//    scanf("%d", &value);
    return 0;
}
