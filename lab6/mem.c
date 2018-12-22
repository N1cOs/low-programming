#include "mem.h"

static mem *start_block = NULL;

/*returns an address of a new block*/
mem *get_pages(mem *current_block, size_t query) {
    const int page_size = getpagesize();
    static uint32_t page_amount = 1;

    size_t needful_page_amount = (query + sizeof(mem) - current_block->capacity) / page_size + 1;
    void *new_chunk = mmap((void *) start_block + page_size * page_amount, needful_page_amount * page_size,
                          PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED | MAP_ANONYMOUS, 0, 0);
    mem *new_block = NULL;
    if (new_chunk == MAP_FAILED) {
        new_block = mmap(NULL, query + sizeof(mem),
                         PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
        if (new_block == MAP_FAILED)
            return NULL;
        new_block->capacity = query;
        new_block->next = MAP_FAILED;
    } else {
        new_block = (mem *) (((uint8_t *) current_block) + query + sizeof(mem));
        new_block->next = NULL;
        new_block->capacity = current_block->capacity + page_size * needful_page_amount - query - sizeof(mem);
    }

    page_amount += needful_page_amount;
    return new_block;
}

void *split_block(mem *current_block, mem *new_block, size_t query) {
    if (current_block->next)
        new_block->next = current_block->next;
    else
        new_block->next = NULL;
    new_block->is_free = true;

    current_block->next = new_block;
    current_block->capacity = query;
    current_block->is_free = false;

    return ((void*) current_block + sizeof(mem));
}

void *_malloc(size_t query) {
    if (query < BLOCK_MIN_SIZE)
        query = BLOCK_MIN_SIZE;

    mem *current_block = start_block;
    while (current_block) {
        if (current_block->is_free) {
            if (current_block->capacity >= query + sizeof(mem)) {
                mem *new_block = (mem *) (((void*) current_block) + query + sizeof(mem));
                new_block->capacity = current_block->capacity - query - sizeof(mem);

                return split_block(current_block, new_block, query);
            } else {
                if (current_block->next) {
                    while (current_block->next)
                        current_block = current_block->next;
                }

                mem *new_block = get_pages(current_block, query);
                if (!new_block)
                    return NULL;

                if (new_block->next == MAP_FAILED) {
                    new_block->next = NULL;
                    new_block->is_free = false;

                    current_block->next = new_block;
                    return ((void *) new_block + sizeof(mem));
                }

                return split_block(current_block, new_block, query);
            }

        } else
            current_block = current_block->next;
    }
    return NULL;
}

void _free(void *address) {
    mem *block = address - sizeof(mem);
    mem *next_block = block->next;
    while(next_block && next_block->is_free){
      block->capacity += next_block->capacity;
      next_block = next_block->next;
    }
    block->next = next_block;
    block->is_free = true;
}


void *heap_init(size_t init_size) {
    const int page_size = getpagesize();

    size_t needful_page_amount = init_size / page_size + 1;
    start_block = mmap(HEAP_START, needful_page_amount * page_size, PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_FIXED | MAP_ANONYMOUS, 0, 0);
    if (start_block == MAP_FAILED)
        start_block = mmap(NULL, needful_page_amount * page_size, PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    if (start_block == MAP_FAILED)
        return NULL;

    start_block->next = NULL;
    start_block->capacity = page_size * needful_page_amount - sizeof(mem);
    start_block->is_free = true;

    return start_block + sizeof(mem);
}
