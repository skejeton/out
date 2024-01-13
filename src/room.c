#include "room.h"
#include "bmath.h"
#include "s2hooks.h"
#include "texture.h"
#include "entity.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

void game_render_room(The_Game *game, Game_Room *room, int x, int y)
{
    Game_Vec2 tilepos;
    int roomw = 9;   
    int roomh = 9;   
    tilepos.x = y*16*(roomh+2);
    tilepos.y = x*16*(roomw+2);
    
    game_remap_to_camera(&tilepos, &game->gameplay.cam);


    if (tilepos.x > game_screen.width) return;
    if (tilepos.y > game_screen.height) return;
    if ((tilepos.x+((roomw+2)*16)) < 0) return;
    if ((tilepos.y+((roomh+2)*16)) < 0) return;

    game_seed(2);
    for (int i = 0; i < roomw+2; i++)
    {
        for (int j = 0; j < roomh+2; j++)
        {

            tilepos.x = i*16+y*16*(roomh+2);
            tilepos.y = j*16+x*16*(roomw+2);
            game_remap_to_camera(&tilepos, &game->gameplay.cam);
            // Stone Tile
            game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 48, 16, 16, 16);

            if (game->gameplay.exit_room->x == y && game->gameplay.exit_room->y == x)
            {
                for (int i = 0; i < 4; i += 1) {
                    int tx = game_random()%32-16;
                    int ty = game_random()%32-16;
                    int variation = game_random()%2*16;
                    game_screen_blit_rect(&game->textures.tileset, tilepos.x+tx, tilepos.y+ty, 48+16, 32+variation, 16, 16);
                }
                game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 48, 32, 16, 16);
                continue;
            }
            if ((room->maze_type != 0) && (i >= 1) && (j >= 1) && (j < roomh+1) && (i < roomw+1))
            {
                int mazeIdx = (room->maze_type-1);
                
                uint32_t pix = game_texture_pixel(&game->textures.mazes, (i-1)+mazeIdx*9, (j-1));
                if ((pix ^ 0xFFFF0000) == 0)
                {
                    game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 48, 48, 16, 16);
                }
                if ((pix ^ 0xFF000000) == 0)
                {
                    game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 48, 64, 16, 16);
                    for (int i = 0; i < 10; i += 1) {
                        int tx = (game_random()%16-4);
                        int ty = (game_random()%16-4);
                        u32 r = game_random();
                        int variation = ((r)%3)*8;
                        int variationy = (r+(int)(game->uptime))%2*8;
                        game_screen_blit_rect(&game->textures.tileset, tilepos.x+tx, tilepos.y+ty, 48+variation, 64+16+variationy, 8, 8);
                    }
                    continue;
                }
            }
            
   
            if ((room->door_left) && i == 0) {
                if (j == roomw/2) {
                    game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 208+16, 0, 16, 16);
                    continue;
                }
                if (j == roomw/2+1)
                {
                    game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 48, 16, 16, 16);
                    continue;
                }
            }
            if (room->door_top && j == 0)
                if (i == roomh/2+1)
                {
                    game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 48, 16, 16, 16);
                    continue;
                }
            if (room->door_right && i == roomh+1) {
                if (j == roomw/2) {
                    game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 208, 0, 16, 16);
                    continue;
                }
                if (j == roomw/2+1)
                {
                    game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 48, 16, 16, 16);
                    continue;
                }
            }
            if (room->door_bottom && j == roomw+1)
                if (i == roomh/2+1)
                {
                    game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 48, 16, 16, 16);
                    continue;
                }

            if (i == 0 && j == 0)
            {
                game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 160, 0, 16, 16);
                continue;
            }
            if (i == (roomw+1) && j == 0)
            {
                game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 160+32, 0, 16, 16);
                continue;
            }
            if (i == 0 && j == (roomh+1))
            {
                game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 160, 32, 16, 16);
                continue;
            }
            if (i == (roomw+1) && j == (roomh+1))
            {
                game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 160+32, 32, 16, 16);
                continue;
            }
            if (i == (roomw+1) && j == (roomh+1))
            {
                game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 192, 32, 16, 16);
                continue;
            }
            if (j == 0)
            {
                game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 160+16, 0, 16, 16);
                continue;
            }
            if (i == 0)
            {
                game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 160, 16, 16, 16);
                continue;
            }
            if (i == (roomw+1))
            {
                game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 192, 16, 16, 16);
                continue;
            }
            if (j == (roomh+1))
            {
                game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 176, 32, 16, 16);
                continue;
            }
        }
    }
}


void game_render_rooms(The_Game *game, Game_Room *cr, int in, int jn)
{
    if (cr)
    {
        game_render_room(game, cr, cr->y, cr->x);
        game_render_rooms(game, cr->bottom, in, jn+1);
        game_render_rooms(game, cr->top, in, jn-1);
        game_render_rooms(game, cr->left, in-1, jn);
        game_render_rooms(game, cr->right, in+1, jn);
    }
    return;
}
    
void game_handle_rooms(The_Game *game)
{

}

bool game_room_check_in(Game_Room *root, int i, int j, int in, int jn)
{
    if (root)
    {
        if (root->x == i && root->y == j)
        {
            return true;
        }
        return game_room_check_in(root->bottom, i, j, in, jn+1) ||
            game_room_check_in(root->top, i, j, in, jn-1) ||
            game_room_check_in(root->left, i, j, in-1, jn) ||
            game_room_check_in(root->right, i, j, in+1, jn);
    }
    return false;
}

void game_rooms_list(Game_Room *room)
{
    return;
}

Game_Room* game_create_rooms_rec(The_Game *game, Game_Room *croom, int in, int jn, int min, int it, Vector *v) {
    if (v->size > 511) return NULL;

    if (it == min)
    {
        return NULL;
    }
    for (int i = 0; i < v->size; i++)
    {
        Game_Vec2 gv = vec_index_ref(v, Game_Vec2, i);
        if((in == gv.x) && (jn == gv.y))
        {
            return NULL;
        }
    }
    Game_Vec2 vr;
    vr.x = in;
    vr.y = jn;

    vec_push_ref(v, vr);

    Game_Room *r = game_create_room(game, in, jn);
    game->gameplay.exit_room = r;
    
    r->x = in;
    r->y = jn;


    if (it == 0)
    {
        game->gameplay.root_room = r;
    }

    if ((game_random()/1000)%2)
    {
        r->bottom = game_create_rooms_rec(game, r->bottom, in, jn+1, min, it+1, v);
        r->door_bottom = false;

        if (r->bottom != NULL)
        {
            r->door_bottom = true;
            r->bottom->door_top = true;
        }
    }

    r->top =    game_create_rooms_rec(game, r->top, in, jn-1, min, it+1, v);
    r->door_top = false;

    

    if (r->top)
    {
        r->door_top = true;
        r->top->door_bottom = true;
    }

    r->left =   game_create_rooms_rec(game, r->left, in-1, jn, min, it+1, v);
    r->door_left = false;

    if (r->left)
    {
        r->door_left = true;
        r->left->door_right = true;
    }

    if ((game_random()/1000)%2)
    {
        r->right =  game_create_rooms_rec(game, r->right, in+1, jn, min, it+1, v);
        r->door_right = false;

        if (r->right)
        {
            r->door_right = true;
            r->right->door_left = true;
        }
    }

    return r;
}

Game_Room* game_create_rooms(The_Game *game, int count)
{
    Vec(GVec2) vec = vec_new(Game_Vec2);
    Game_Room *room = game_create_rooms_rec(game, game->gameplay.root_room, 0, 0, count, 0, &vec);
    if (game->gameplay.exit_room)  
        game->gameplay.exit_room->maze_type = 0;
    vec_drop(vec);
    return room;
}

void game_create_room_colliders(The_Game *game, Game_Room *r)
{
    if (r)
    {     
        if (game_random()%2 && game->gameplay.entity_count < 200)
            game_add_zombie(game, 16*2+(r->x*16*11), 16*2+(r->y*16*11));

        for (int i = 1; i < 10; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Game_Collider c;
                c.r.y = 0;
                c.r.x = 0;
                if (j == 0)
                    c.r.x = 16*i;
                if (j == 1)
                    c.r.y = 16*i;
                if (j == 2)
                {
                    c.r.y = (10*16);
                    c.r.x = 16*i;
                }
                if (j == 3)
                {
                    c.r.x = (10*16);
                    c.r.y = 16*i;
                }
                c.solid = true;
                c.active = true;
                
                if (i == 9/2+1)
                {
                    if (j == 0 && r->door_top)
                        c.active = c.solid = false;
                    if (j == 1 && r->door_left)
                        c.active = c.solid = false;
                    if (j == 2 && r->door_bottom)
                        c.active = c.solid = false;
                    if (j == 3 && r->door_right)
                        c.active = c.solid = false;
                }
                c.r.x += r->x*16*11;
                c.r.y += r->y*16*11;
                c.r.w = 16;
                c.r.h = 16;
                c.projectile = false;
                c.zombie = false;
                vec_push(game->gameplay.colliders, c);
            }
        }

        
        if (r->maze_type != 0)
        {
            int mazeIdx = (r->maze_type-1);
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    uint32_t pix = game_texture_pixel(&game->textures.mazes, i+mazeIdx*9, j);
                    if ((pix ^ 0xFFFF0000) == 0)
                    {
                        Game_Collider c;
                        c.r.x = 16*i+16+r->x*16*11;
                        c.r.y = 16*j+16+r->y*16*11;
                        c.r.w = 16;
                        c.r.h = 16;
                        c.solid = false;
                        c.projectile = false;
                        c.zombie = true;
                        c.active = true;
                        vec_push(game->gameplay.colliders, c);
                    }
                    if ((pix ^ 0xFF000000) == 0)
                    {
                        Game_Collider c;
                        c.r.x = 16*i+16+r->x*16*11;
                        c.r.y = 16*j+16+r->y*16*11;
                        c.r.w = 16;
                        c.r.h = 16;
                        c.solid = true;
                        c.projectile = false;
                        c.zombie = false;
                        c.active = true;
                        vec_push(game->gameplay.colliders, c);

                    }
                }
            }
        }

        game_create_room_colliders(game, r->top);
        game_create_room_colliders(game, r->bottom);
        game_create_room_colliders(game, r->left);
        game_create_room_colliders(game, r->right);
    }
}

Game_Room* game_create_room(The_Game *game, int x, int y)
{
    // Room
    Game_Room room = {
        .top = NULL,
        .bottom = NULL,
        .left = NULL,
        .right = NULL,
        .maze_type = 0,
        .door_right = false,
        .door_top = false,
        .door_bottom = false,
        .door_left = false,
        .zombies = 0,
        .x = 0,
        .y = 0,
    };  
    Game_Room *res = new(Game_Room, 1);
    *res = room;
    if ((game_random()/1000)%2 == 0)
    {
        res->maze_type = (game_random()/1000)%((game->textures.mazes.width/9)+1);

    }

    
    return res;
}
