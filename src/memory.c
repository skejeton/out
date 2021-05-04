#include "memory.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>

void* mem_alloc(Size size) {
    return malloc(size);
}

u32 memleaks;

void mem_free(void *ptr) {
    free(ptr);
}

void mem_purge(void *ptr) {
    free(ptr);
}
