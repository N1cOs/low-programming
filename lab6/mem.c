#include "mem.h"

static mem *start_block = NULL;

uint32_t get_pages(size_t query) {
    const int page_size = getpagesize();
    static uint32_t page_amount = 1;

    size_t chunk_length = query / page_size + 1;
    void *new_chunk = mmap((void *) start_block + page_size * page_amount, chunk_length * page_size,
                           PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED | MAP_ANONYMOUS, 0, 0);
    if (!new_chunk)
        new_chunk = mmap(NULL, chunk_length, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    if (new_chunk) {
        page_amount += chunk_length;
        return chunk_length;
    }
    return -1;

}

void *_malloc(size_t query) {
    const int page_size = getpagesize();

    if (query < BLOCK_MIN_SIZE)
        query = BLOCK_MIN_SIZE;

    mem *current_block = start_block;
    while (current_block) {
        if (current_block->is_free) {
            if (current_block->capacity >= query + sizeof(mem)) {
                mem *new_block = (mem *) (((uint8_t *) current_block) + query + sizeof(mem));

                if (current_block->next)
                    new_block->next = current_block->next;
                else
                    new_block->next = NULL;
                new_block->capacity = current_block->capacity - query - sizeof(mem);
                new_block->is_free = true;

                current_block->next = new_block;
                current_block->capacity = query;
                current_block->is_free = false;

                return ((uint8_t *) current_block + sizeof(mem));
            } else {
                if (current_block->next) {
                    while (current_block->next)
                        current_block = current_block->next;
                }
                uint32_t page_amount = get_pages(query + sizeof(mem) - current_block->capacity);
                if (page_amount == -1)
                    return NULL;

                mem *new_block = (mem *) (((uint8_t *) current_block) + query + sizeof(mem));
                new_block->next = NULL;
                new_block->capacity = current_block->capacity + page_size * page_amount - query - sizeof(mem);
                new_block->is_free = true;

                current_block->next = new_block;
                current_block->capacity = query;
                current_block->is_free = false;

                return ((uint8_t *) current_block + sizeof(mem));
            }
        } else {
            current_block = current_block->next;
        }
    }
    return NULL;
}

void _free(void *address) {
    mem *block = address - sizeof(mem);
    mem *next_block = block->next;
    if (next_block && next_block->is_free) {
        block->capacity += next_block->capacity;
        block->next = next_block->next;
    }
    block->is_free = true;
}


void *heap_init(size_t init_size) {
    const int page_size = getpagesize();

    size_t chunk_length = init_size / page_size + 1;
    void *ptr = mmap(HEAP_START, chunk_length * page_size, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_FIXED | MAP_ANONYMOUS, 0, 0);
    if (!ptr)
        return ptr;

    start_block = (mem *) ptr;
    start_block->next = NULL;
    start_block->capacity = page_size - sizeof(mem);
    start_block->is_free = true;
    return start_block + sizeof(mem);
}

