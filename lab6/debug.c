#include "mem.h"

void _malloc_debug_info( FILE *f, mem *address ) {
    size_t i;
    fprintf( f,"start: %p\nsize: %lu\nis_free: %d\n", (void*)address, address-> capacity, address-> is_free );
    for ( i = 0;i <  DEBUG_FIRST_BYTES  &&  i <  address-> capacity; i++)
        fprintf( f, "%hhX",((uint8_t *)address)[ sizeof( mem ) + i ] );
    putc( '\n', f );
}

void _malloc_debug_heap( FILE *f, mem *ptr ) {
    for( ; ptr; ptr = ptr->next )
        _malloc_debug_info( f, ptr );
}

int main(){
    FILE *debug_file = fopen("/home/nick/Documents/LowProgramming/test_project/lab6/test.txt", "w");
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
    fclose(debug_file);
    return 0;
}
