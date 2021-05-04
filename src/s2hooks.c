#include "s2hooks.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_shape.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdlib.h>

File read_file(char *path) {
    FILE *file = fopen(path, "r");
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    

    char *buf = malloc(fsize + 1);
    fread(buf, 1, fsize, file);
    fclose(file);

    return (File) {
        .size = fsize,
        .buf = buf 
    };
}

char convert_scancode_to_char(Byte code) {
    return code;
}
