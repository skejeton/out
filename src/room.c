#include "room.h"
#include "bmath.h"
#include "s2hooks.h"
#include "texture.h"
#include "entity.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "diagnostics.h"

void game_render_room(The_Game *game, Game_Room *room, int x, int y)
{
    Game_Vec2 tilepos;
    int roomw = 9;   
    int roomh = 9;   
    tilepos.x = y*16*(roomh+2);
    tilepos.y = x*16*(roomw+2);

    game_global_diagnostics.total_rooms += 1;
    
    game_remap_to_camera(&tilepos, &game->gameplay.cam);

    bool oob = tilepos.x > game_screen.width ||
        tilepos.y > game_screen.height ||
        (tilepos.x+((roomw+2)*16)) < 0 ||
        (tilepos.y+((roomh+2)*16)) < 0;

    if (oob) {
        game_global_diagnostics.skipped_rooms += 1;
        return;
    }

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
            
   
            if (room->left && i == 0) {
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
            if (room->top && j == 0)
                if (i == roomh/2+1)
                {
                    game_screen_blit_rect(&game->textures.tileset, tilepos.x, tilepos.y, 48, 16, 16, 16);
                    continue;
                }
            if (room->right && i == roomh+1) {
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
            if (room->bottom && j == roomw+1)
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


void game_render_rooms(The_Game *game, Vec(Game_Vec2) *visited, Game_Room *cr, int in, int jn)
{
    if (!cr) return;

    for (int i = 0; i < visited->size; i++)
    {
        Game_Vec2 gv = vec_index_ref(visited, Game_Vec2, i);
        if((cr->x == gv.x) && (cr->y == gv.y))
        {
            return;
        }
    }

    vec_push_ref(visited, ((Game_Vec2){cr->x, cr->y}));

    if (cr)
    {
        game_render_room(game, cr, cr->y, cr->x);
        game_render_rooms(game, visited, cr->bottom, in, jn+1);
        game_render_rooms(game, visited, cr->top, in, jn-1);
        game_render_rooms(game, visited, cr->left, in-1, jn);
        game_render_rooms(game, visited, cr->right, in+1, jn);
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

Game_Room* game_create_rooms_rec(The_Game *game, Game_Room *croom, int in, int jn, int max, int it, int *hoer, Vector *v) {
    if (v->size > 511 || v->size > max) return NULL;
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
    if (!game->gameplay.exit_room) {
        game->gameplay.exit_room = r;
    }
    if (it > *hoer) {
        game->gameplay.exit_room = r;
        *hoer = it;
    }
    
    r->x = in;
    r->y = jn;

    if (it == 0)
    {
        game->gameplay.root_room = r;
    }

    int dircount = game_random()%4+1;
    bool dirs[4] = { !!r->bottom, !!r->top, !!r->left, !!r->right };

    for (int i = 0; i < dircount; i++)
    {
        // find available random dir 
        int dir = game_random()%4;
        while (dirs[dir])
        {
            dir = game_random()%4;
        }

        switch (dir)
        {
            case 0:
                r->bottom = game_create_rooms_rec(game, r->bottom, in, jn+1, max, it+1, hoer, v);
                if (r->bottom != NULL)
                {
                    dirs[dir] = true;
                    r->bottom->top = r;
                }
                break;
            case 1:
                r->top = game_create_rooms_rec(game, r->top, in, jn-1, max, it+1, hoer, v);
                if (r->top != NULL)
                {
                    dirs[dir] = true;
                    r->top->bottom = r;
                }
                break;
            case 2:
                r->left = game_create_rooms_rec(game, r->left, in-1, jn, max, it+1, hoer, v);
                if (r->left != NULL)
                {
                    dirs[dir] = true;
                    r->left->right = r;
                }
                break;
            case 3:
                r->right = game_create_rooms_rec(game, r->right, in+1, jn, max, it+1, hoer, v);
                if (r->right != NULL)
                {
                    dirs[dir] = true;
                    r->right->left = r;
                }
                break;
        }
    }

    return r;
}

Game_Room* game_create_rooms(The_Game *game, int count)
{
    Vec(GVec2) vec = vec_new(Game_Vec2);
    int hoer = 0;
    Game_Room *room = game_create_rooms_rec(game, game->gameplay.root_room, 0, 0, count, 0, &hoer, &vec);
    assert(game->gameplay.exit_room);
    game->gameplay.exit_room->maze_type = 0;
    vec_drop(vec);
    return room;
}

void game_create_room_colliders(The_Game *game, Game_Room *r, Vec(Game_Vec2) *visited)
{
    if (!r) {
        return;
    }

    for (int i = 0; i < visited->size; i++)
    {
        Game_Vec2 gv = vec_index_ref(visited, Game_Vec2, i);
        if((r->x == gv.x) && (r->y == gv.y))
        {
            return;
        }
    }

    vec_push_ref(visited, ((Game_Vec2){r->x, r->y}));

    if (r)
    {     
        if (game_random()%2 && game->gameplay.entity_count < 200)
            game_add_zombie(game, 16*2+(r->x*16*11), 16*2+(r->y*16*11));
        
        if (!r->left) {
            Game_Collider c;
            c.r.x = r->x*16*11;
            c.r.y = r->y*16*11;
            c.r.w = 16;
            c.r.h = 16*11;
            c.solid = true;
            c.projectile = false;
            c.zombie = false;
            c.active = true;
            vec_push(game->gameplay.colliders, c);
        } else {
            Game_Collider c;
            c.r.x = r->x*16*11;
            c.r.y = r->y*16*11;
            c.r.w = 16;
            c.r.h = 16*5;
            c.solid = true;
            c.projectile = false;
            c.zombie = false;
            c.active = true;
            vec_push(game->gameplay.colliders, c);
            c.r.y += 16*6;
            vec_push(game->gameplay.colliders, c);
        }
        if (!r->right) {
            Game_Collider c;
            c.r.x = r->x*16*11+16*10;
            c.r.y = r->y*16*11;
            c.r.w = 16;
            c.r.h = 16*11;
            c.solid = true;
            c.projectile = false;
            c.zombie = false;
            c.active = true;
            vec_push(game->gameplay.colliders, c);
        } else {
            Game_Collider c;
            c.r.x = r->x*16*11+16*10;
            c.r.y = r->y*16*11;
            c.r.w = 16;
            c.r.h = 16*5;
            c.solid = true;
            c.projectile = false;
            c.zombie = false;
            c.active = true;
            vec_push(game->gameplay.colliders, c);
            c.r.y += 16*6;
            vec_push(game->gameplay.colliders, c);
        }
        if (!r->top) {
            Game_Collider c;
            c.r.x = r->x*16*11;
            c.r.y = r->y*16*11;
            c.r.w = 16*11;
            c.r.h = 16;
            c.solid = true;
            c.projectile = false;
            c.zombie = false;
            c.active = true;
            vec_push(game->gameplay.colliders, c);
        } else {
            Game_Collider c;
            c.r.x = r->x*16*11;
            c.r.y = r->y*16*11;
            c.r.w = 16*5;
            c.r.h = 16;
            c.solid = true;
            c.projectile = false;
            c.zombie = false;
            c.active = true;
            vec_push(game->gameplay.colliders, c);
            c.r.x += 16*6;
            vec_push(game->gameplay.colliders, c);
        }
        if (!r->bottom) {
            Game_Collider c;
            c.r.x = r->x*16*11;
            c.r.y = r->y*16*11+16*10;
            c.r.w = 16*11;
            c.r.h = 16;
            c.solid = true;
            c.projectile = false;
            c.zombie = false;
            c.active = true;
            vec_push(game->gameplay.colliders, c);
        } else {
            Game_Collider c;
            c.r.x = r->x*16*11;
            c.r.y = r->y*16*11+16*10;
            c.r.w = 16*5;
            c.r.h = 16;
            c.solid = true;
            c.projectile = false;
            c.zombie = false;
            c.active = true;
            vec_push(game->gameplay.colliders, c);
            c.r.x += 16*6;
            vec_push(game->gameplay.colliders, c);
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
                        c.r.x = 16*i+16+r->x*16*11+3;
                        c.r.y = 16*j+16+r->y*16*11+3;
                        c.r.w = 16-6;
                        c.r.h = 16-6;
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

        game_create_room_colliders(game, r->top, visited);
        game_create_room_colliders(game, r->bottom, visited);
        game_create_room_colliders(game, r->left, visited);
        game_create_room_colliders(game, r->right, visited);
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
