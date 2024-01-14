#pragma once
#include "s2hooks.h"
#include "texture.h"
#include <math.h>

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

static inline Game_Vec2 game_rect_vs_rect_snap(Game_Rect r, Game_Rect along)
{
    double x_left = r.x + r.w - along.x;
    double y_up = r.y + r.h - along.y;

    double x_right = along.x + along.w - r.x;
    double y_down = along.y + along.h - r.y;

    double min_x = 0, min_y = 0;

    if (fabs(x_left) < fabs(x_right))
    {
        min_x = -x_left;
    }
    else
    {
        min_x = x_right;
    }

    if (fabs(y_up) < fabs(y_down))
    {
        min_y = -y_up;
    }
    else
    {
        min_y = y_down;
    }

    if (fabs(min_x) < fabs(min_y))
    {
        return (Game_Vec2){min_x, 0};
    }
    return (Game_Vec2){0, min_y};
}

void game_seed(int ns);
int game_random();
