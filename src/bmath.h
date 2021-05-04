#pragma once
#include "s2hooks.h"
#include "texture.h"

typedef struct 
{
    double x;
    double y;
} Game_Camera;

typedef struct 
{
    double x;
    double y;
} Game_Vec2;

typedef struct
{
    double x;
    double y;
    double w;
    double h;
} Game_Rect;

typedef struct
{
    Game_Rect r;
    bool solid;
    bool projectile;
    bool zombie;
    bool active;
} Game_Collider;

void game_remap_to_camera(Game_Vec2 *orig, Game_Camera *cam);

bool game_check_collision(Game_Collider *c1, Game_Collider *c2);
Game_Vec2* game_normalize_vector(Game_Vec2 *vec);

void game_seed(int ns);
int game_random();
