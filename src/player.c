#include "player.h"
#include "bmath.h"
#include "entity.h"
#include <stdio.h>

/// BAD
void game_player_handle(The_Game *game)
{
    game->gameplay.cam.x = game->gameplay.player.pos.x;
    game->gameplay.cam.y = game->gameplay.player.pos.y;

    double sp = 10;
    
    if (game->keys.a2)
    {
        sp = 20;
    }

    bool pressed = false;
    if (game->keys.w)
    {
        game->gameplay.player.accel.y = -sp;
        game->gameplay.player.vel.y += game->gameplay.player.accel.y;
        game->gameplay.player.dir = 0;
        pressed = true;
    }
    if (game->keys.a)
    {
        game->gameplay.player.accel.x = -sp;
        game->gameplay.player.vel.x += game->gameplay.player.accel.x;
        game->gameplay.player.dir = 1;
        pressed = true;
    }
    if (game->keys.s)
    {
        game->gameplay.player.accel.y = sp;
        game->gameplay.player.vel.y += game->gameplay.player.accel.y;
        game->gameplay.player.dir = 2;
        pressed = true;
    }
    if (game->keys.d)
    {

        game->gameplay.player.accel.x = sp;
        game->gameplay.player.vel.x += game->gameplay.player.accel.x;
        game->gameplay.player.dir = 3;
        pressed = true;
    }
    Game_Collider sc;
    sc.r.x = game->gameplay.player.pos.x+game->gameplay.player.vel.y*game->delta+2;
    sc.r.y = game->gameplay.player.pos.y+game->gameplay.player.vel.y*game->delta+10;
    sc.r.w = 16-4;
    sc.r.h = 16-10;

    game->gameplay.player.state = PLAYER_STATE_WALK;
    if (!pressed)
    {
        game->gameplay.player.state = PLAYER_STATE_IDLE;
    game->gameplay.player.vel.y /= 1.1;
    }
    for (int i = 0; i < (game->gameplay.colliders.size); i++)
    {
        Game_Collider srect = vec_index(game->gameplay.colliders, Game_Collider, i);
        bool col = game_check_collision(&srect, &sc);
        if (col)
        {
            if (srect.solid)
            {
                Game_Vec2 displacement = {game->gameplay.player.pos.x+game->gameplay.player.vel.x*game->delta,
                        game->gameplay.player.pos.y+game->gameplay.player.vel.y*game->delta};
                Game_Vec2 difference = {(displacement.x - srect.r.x), (displacement.y - srect.r.y)};
                game->gameplay.player.vel.x = difference.x;
                game->gameplay.player.vel.y = difference.y;
            }
            else if (srect.zombie && srect.active)
            {
                game->state = 3;
            }
        }
    }

    if (game->gameplay.player.vel.x > 10*sp) game->gameplay.player.vel.x = 100;
    if (game->gameplay.player.vel.x < -10*sp) game->gameplay.player.vel.x = -100;
    if (game->gameplay.player.vel.y > 10*sp) game->gameplay.player.vel.y = 100;
    if (game->gameplay.player.vel.y < -10*sp) game->gameplay.player.vel.y = -100;

    game->gameplay.player.pos.x += game->gameplay.player.vel.x*game->delta;
    game->gameplay.player.pos.y += game->gameplay.player.vel.y*game->delta;

    if (!game->keys.a1 && game->keys.a2)
    {
        if (game->gameplay.player.dir == 1)
            game_add_projectile(game, game->gameplay.player.pos.x, game->gameplay.player.pos.y, -100, 0);
        if (game->gameplay.player.dir == 3)
            game_add_projectile(game, game->gameplay.player.pos.x, game->gameplay.player.pos.y, 100, 0);
        if (game->gameplay.player.dir == 0)
            game_add_projectile(game, game->gameplay.player.pos.x, game->gameplay.player.pos.y, 0, -100);
        if (game->gameplay.player.dir == 2)
            game_add_projectile(game, game->gameplay.player.pos.x, game->gameplay.player.pos.y, 0, 100);
    }

    game->gameplay.player.accel.x = 0;
    game->gameplay.player.accel.y = 0;

    game->gameplay.player.vel.x /= 1.1;
    game->gameplay.player.vel.y /= 1.1;
}

void game_player_render(The_Game *game)
{
    Game_Vec2 r = { game->gameplay.player.pos.x, game->gameplay.player.pos.y };   
    game_remap_to_camera(&r, &game->gameplay.cam);
    
    if (game->gameplay.player.state == PLAYER_STATE_IDLE)
    {
        if (game->gameplay.player.dir == 1)
        {
            game_screen_blit_rect(&game->textures.tileset, r.x, r.y, 0, 0+16, 16, 16);
        }
        else if (game->gameplay.player.dir == 3)
        {
            game_screen_blit_rect(&game->textures.tileset, r.x, r.y, 0, 0, 16, 16);
        }
        else if (game->gameplay.player.dir == 0)
        {
            game_screen_blit_rect(&game->textures.tileset, r.x, r.y, 0, 0+32, 16, 16);
        }
        else if (game->gameplay.player.dir == 2)
        {
            game_screen_blit_rect(&game->textures.tileset, r.x, r.y, 0, 0+64, 16, 16);
        }
    }
    else
    {       
        if (game->gameplay.player.dir == 1)
        {
            game_screen_blit_rect(&game->textures.tileset, r.x, r.y, ((int)(game->uptime * 5) % 2) ? 0 : 32, 0+16, 16, 16);
        }
        else if (game->gameplay.player.dir == 3)
        {
            game_screen_blit_rect(&game->textures.tileset, r.x, r.y, ((int)(game->uptime * 5) % 2) ? 0 : 32, 0, 16, 16);
        }
        else if (game->gameplay.player.dir == 0)
        {
            game_screen_blit_rect(&game->textures.tileset, r.x, r.y, ((int)(game->uptime * 5) % 3)*16, 0+48, 16, 16);
        }
        else if (game->gameplay.player.dir == 2)
        {
            game_screen_blit_rect(&game->textures.tileset, r.x, r.y, ((int)(game->uptime * 5) % 3)*16, 0+80, 16, 16);
        }  
    }
    
}
