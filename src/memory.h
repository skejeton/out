
#pragma once
#include "types.h"

void* mem_alloc(Size size);

extern u32 memleaks;

void mem_free(void *ptr);

void mem_purge(void *ptr);
