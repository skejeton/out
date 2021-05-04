#include "s2hooks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vector vector_create(Size typeSize)
{
    Vector r;
    r.data = new_sized(typeSize, VECTOR_DEFAULT_CAPACITY);
    r.capacity = VECTOR_DEFAULT_CAPACITY;
    r.size = 0;
    r.type_size = typeSize;
    return r;
}

int vector_push(Vector *vec, void* v)
{
    if ((vec->size) > vec->capacity)
    {
        u32 pcapacity = vec->capacity;
        vec->capacity += VECTOR_DEFAULT_CAPACITY;
        void* newd = new_sized(vec->type_size, vec->capacity);
        memcpy(newd, vec->data, pcapacity*vec->type_size);
        delete(vec->data);
        vec->data = newd;
    }

    memcpy(ptr_at(vec->data, vec->type_size, vec->size), v, vec->type_size);

    vec->size += 1;
    return vec->size - 1;
}

void* vector_pop(Vector *vec)
{
    if (vec->size < 1)
    {
        return NULL;
    }
    vec->size -= 1;
    return vector_index(vec, vec->size);
}

inline void* vector_index(Vector *vec, Size idx)
{
    if (idx > vec->size) {
        fprintf(stderr, "Vector access out of bounds, size %d requested %d\n", vec->size, idx);
        exit(-1);
    }
    return ptr_at(vec->data, vec->type_size, idx);
}

void vector_drop(Vector *vec)
{
    delete(vec->data);
}

