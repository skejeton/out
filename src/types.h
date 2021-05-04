#pragma once
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define TRACE(n) EM_ASM({console.log("TRACE ", String.fromCharCode($0));}, n)
#else
#define TRACE(n) printf("TRACE: %c\n", (n))
#endif



#include <stddef.h>
typedef unsigned char Byte;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef char i8;
typedef short i16;
typedef int i32;

typedef unsigned int Time;

#define bool char
#define true 1
#define false 0
#define ptr_at(ptr, step, idx) (void*)(((Byte*)(ptr))+((step)*(idx)))
#define new(type, len) (type*)mem_alloc(sizeof(type)*(len))
// new by specified size
#define new_sized(typesz, len) mem_alloc((typesz)*(len))
#define delete(ptr) mem_free(ptr)

typedef size_t Size;
