#include "bmp.h"
#include <stdlib.h>
#include <stdio.h>
#include "texture.h"
#include "types.h"

Game_Texture game_read_bmp(char *path)
{
    SDL_Surface *s = SDL_LoadBMP(path);
    if (!s)
    {
        printf("error loading bmp: %s\n", SDL_GetError());
        exit(1);
    }
    return game_texture_from_surface(s);
}
