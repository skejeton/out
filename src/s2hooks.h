/*!
 *  Hooks for System2 functions, types & macros
 */
#pragma once
#include "types.h"

#define Vec(T) Vector
#define vec_new(T) vector_create(sizeof(T))
#define vec_foreach(v, idx) for (int idx = 0; idx < (v).size; idx += 1)
#define vec_index(v, T, i) *((T*)vector_index(&v, i))
#define vec_index_ref(v, T, i) *((T*)vector_index(v, i))
#define vec_index_ptr(v, T, i) ((T*)vector_index(&v, i))
#define vec_index_ref_ptr(v, T, i) ((T*)vector_index(v, i))
#define vec_push(v, value) vector_push(&v, &value)
#define vec_push_ref(v, value) vector_push(v, &value)
#define vec_pop(v) *((T*)vector_pop(&v))
#define vec_drop(v) vector_drop(&v)

/**
 * FS
 */

typedef struct
{
    u32 size;
    Byte *buf;
} File;

File read_file(char *path);


/**
 *  Memory
 */

#include "memory.h"

/**
 *  Vector
 */

#define VECTOR_DEFAULT_CAPACITY 100000
typedef struct
{
    void *data;
    Size size, type_size, capacity;
} Vector;

Vector vector_create(Size typeSize);

int vector_push(Vector *vec, void* v);

void* vector_pop(Vector *vec);

void* vector_index(Vector *vec, Size idx);

void vector_drop(Vector *vec);

/**
 * Event
 */

#include "event.h"

#include "ps2.h"
