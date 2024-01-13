#include "texture.h"
#include "event.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdlib.h>

Game_Texture game_screen;
Game_Texture game_font;
char *game_fontmap;
char *game_alt_fontmap;

static SDL_Renderer *renderer = NULL;
static SDL_Window *window = NULL;

void init_renderer() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("IIIIIIIIIIIITNEVMKXVMKMkrenvskrnvEKSVMCVKMCKV<MKC<MVKMCVKVKKVKKKVVKVKVNCKENVKCENXKENVKRENKSTRTn", 
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       320*2, 200*2, SDL_WINDOW_RESIZABLE);
 
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, 0);
    window = win;
    renderer = rend;
}

void game_screen_init()
{
    init_renderer();
    game_screen = (Game_Texture) { .width = 320, .height = 200 }; 
}

void game_screen_write(char *str, int x, int y)
{
    int left = x;
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == ' ')
        {
            x += 8;
            i++;
            continue;
        }
        if (str[i] == '\n') {
            y += 8;
            x = left;
            i++;
            continue;
        }
        int idx = 0;
        while (1)
        {
            if ((str[i] == game_fontmap[idx]) || (str[i] == game_alt_fontmap[idx]))
            {
                break;
            }
            if (game_fontmap[idx] == '\0')
            {
                idx = 39;
                break;
            }
            idx += 1;
        }

        game_screen_blit_rect(&game_font, x, y, idx*8, 0, 8, 8);
        x += 8;
        i++;
    }
}

void game_font_init(Game_Texture tex, char *fontmap, char *altfontmap)
{
    game_font = tex;
    fontmap = fontmap;
    game_alt_fontmap = altfontmap;
    game_fontmap = fontmap;
}

void game_screen_clear(int color)
{
    SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xFF, (color >> 8) & 0xFF, (color >> 0) & 0xFF, (color >> 24) & 0xFF);
    SDL_RenderClear(renderer);
}
/*
u32 double_colors_to_u32(double a, double r, double g, double b) {
    u32 a8 = (u32)(a * 255.0) & 0xFF;
    u32 r8 = (u32)(r * 255.0) & 0xFF;
    u32 g8 = (u32)(g * 255.0) & 0xFF;
    u32 b8 = (u32)(b * 255.0) & 0xFF;
    
    return a8 << 24 | r8 << 16 | g8 << 8 | b8;
}

#define ASSERT(c) if (!(c)) { printf("Assertion error: " #c "\n"); exit(-1); }

u32 blend_colors(u32 a, u32 b) {
    // Normalize colors
    double a_alpha = ((double)((a>>24)&0xFF))/255.0;
    double b_alpha = ((double)((b>>24)&0xFF))/255.0;
    double o_alpha = a_alpha+b_alpha*(1.0-a_alpha);
    if (o_alpha == 0.0) return 0xFF000000; 

    double a_red   = ((double)(a >> 16 & 0xFF)) / 255.0,
          a_green = ((double)(a >> 8  & 0xFF)) / 255.0,
          a_blue  = ((double)(a >> 0  & 0xFF)) / 255.0;

    double b_red   = ((double)(b >> 16 & 0xFF)) / 255.0,
          b_green = ((double)(b >> 8  & 0xFF)) / 255.0,
          b_blue  = ((double)(b >> 0  & 0xFF)) / 255.0;

    double o_red   = (a_red  *a_alpha+b_red  *b_alpha*(1.0-a_alpha))/o_alpha;
    double o_green = (a_green*a_alpha+b_green*b_alpha*(1.0-a_alpha))/o_alpha;
    double o_blue  = (a_blue *a_alpha+b_blue *b_alpha*(1.0-a_alpha))/o_alpha;

    return double_colors_to_u32(o_alpha, o_red, o_green, o_blue);
}*/

void game_screen_blit_rect(Game_Texture *tex, int x, int y, int ix, int iy, int iw, int ih)
{
#ifndef __EMSCRIPTEN__
    int w;
    int h;
    SDL_GetWindowSize(window, &w, &h);
    int xo = w/2-(scalex*320)/2;
    int yo = h/2-(scaley*200)/2;
    SDL_Rect clip_rect = { .x = xo, .y = yo, .w = scalex*320, .h = scaley*200 };
    SDL_RenderSetClipRect(renderer, &clip_rect);
    SDL_Rect r = { .x = ix, .y = iy, .w = iw, .h = ih };
    SDL_Rect d = { .x = xo+x*scalex, .y = yo+y*scaley, .w = iw*scalex, .h = ih*scaley };

    SDL_RenderCopy(renderer, tex->texture, &r, &d);
#else
    SDL_Rect r = { .x = ix, .y = iy, .w = iw, .h = ih };
    SDL_Rect d = { .x = x*2, .y = y*2, .w = iw*2, .h = ih*2 };

    SDL_RenderCopy(renderer, tex->texture, &r, &d);
#endif
}

void game_texture_drop(Game_Texture texture) {
    SDL_FreeSurface(texture.surface);
    SDL_DestroyTexture(texture.texture);
}

/// Thanks, stackoverflow
void flip_surface(SDL_Surface* surface) 
{
    SDL_LockSurface(surface);
    
    int pitch = surface->pitch; // row size
    char* temp = new(char, pitch); // intermediate buffer
    char* pixels = (char*) surface->pixels;
    
    for(int i = 0; i < surface->h / 2; ++i) {
        // get pointers to the two rows to swap
        char* row1 = pixels + i * pitch;
        char* row2 = pixels + (surface->h - i - 1) * pitch;
        
        // swap rows
        memcpy(temp, row1, pitch);
        memcpy(row1, row2, pitch);
        memcpy(row2, temp, pitch);
    }
    
    delete(temp);

    SDL_UnlockSurface(surface);
}

Game_Texture game_texture_from_surface(SDL_Surface *surface) {
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
    return (Game_Texture) {
        .width = surface->w,
        .height = surface->h,
        .texture = tex,
        .surface = surface
    };
}

void game_screen_blit_at(Game_Texture *tex, int x, int y)
{
    game_screen_blit_rect(tex, x, y, 0, 0, tex->width, tex->height);
}

void game_screen_rect(int x, int y, int width, int height, int color)
{
    SDL_Rect r = { .x = x, .y = y, .w = width, .h = height };
    SDL_RenderFillRect(renderer, &r);
}

void game_screen_paint()
{
    SDL_RenderPresent(renderer);
}

void game_screen_drop() {
    SDL_DestroyRenderer(renderer);
 
    SDL_DestroyWindow(window);
     
    SDL_Quit();
}

