#pragma once
#include "s2hooks.h"
#include "types.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#define game_texture_pixel(tex, x, y) ((u32*)((tex)->surface->pixels))[(x)+(y)*(tex)->width]

typedef struct {
    SDL_Surface *surface;
    SDL_Texture *texture;
    i32 width;
    i32 height; 
} Game_Texture;

extern Game_Texture game_screen;
extern Game_Texture game_font;
extern char *game_fontmap;
extern char *game_alt_fontmap;

void game_font_init(Game_Texture tex, char *fontmap, char *altfontmap);
void game_screen_init();
void game_screen_blit(Game_Texture *tex);
void game_texture_drop(Game_Texture texture);
void game_screen_measure(char *str, int *w, int *h);
void game_screen_write(char *str, i32 x, i32 y);
Game_Texture game_texture_from_surface(SDL_Surface *surface);
void game_screen_blit_rect(Game_Texture *tex, i32 x, i32 y, i32 ix, i32 iy, i32 iw, i32 ih);
void game_screen_clear(i32 color);
void game_screen_blit_at(Game_Texture *tex, i32 x, i32 y);
void game_screen_rect(i32 x, i32 y, i32 width, i32 height, i32 color);
void game_screen_paint();
void game_screen_drop();
