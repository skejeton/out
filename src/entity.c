#include "entity.h"
#include "bmath.h"
#include "gamestruct.h"
#include <stdio.h>

Game_Collider *entity_collider(The_Game *game, Game_Entity *entity) {
    return vec_index_ptr(game->gameplay.colliders, Game_Collider, entity->collider_id);
}

void game_add_projectile(The_Game *game, double x, double y, double vx, double vy)
{
    Game_Collider collider = {
        .active = true,
        .zombie = false,
        .projectile = true,
        .solid = false,
        .r = { x, y, 8, 8 }
    };

    Game_Entity e = { 
        .entity_type = GAME_ENTITY_PROJECTILE,
        .collider_id = vec_push(game->gameplay.colliders, collider),
        .dir = 0,
        .pos = { x, y },
        .vel = { vx, vy },
        .state = ENTITY_STATE_IDLE
    };


    for (int i = 0; i < 256; i++)
    {
        if (game->gameplay.entities[i].entity_type == GAME_ENTITY_NONE)
        {
            game->gameplay.entities[i] = e;
            game->gameplay.entity_count += 1;
            return;
        }
    }
    printf("Failed to summon a projectile, what that means is that a collider has been leaked! Too bad!\n");
}

void game_add_zombie(The_Game *game, double x, double y)
{
    Game_Collider collider = {
        .active = true,
        .zombie = true,
        .projectile = false,
        .solid = false,
        .r = { x+2, y+10, 16-4, 16-10 }
    };

    
    
    
    Game_Entity e;

    e.collider_id = vec_push(game->gameplay.colliders, collider);
    e.dir = 0;
    e.entity_type = GAME_ENTITY_ZOMBIE;
    e.pos.x = x;
    e.pos.y = y;
    e.vel.x = 0;
    e.vel.y = 0;
    e.state = ENTITY_STATE_IDLE;

    for (int i = 0; i < 256; i++)
    {
        if (game->gameplay.entities[i].entity_type == GAME_ENTITY_NONE)
        {
            game->gameplay.entities[i] = e;
            game->gameplay.entity_count += 1;
            return;
        }
    }
    printf("Failed to summon a zombie, what that means is that a collider has been leaked! Too bad!\n");
}

void game_handle_entities(The_Game *game)
{
    for (int i = 0; i < 256; i++)
    {
        Game_Entity *entity = &game->gameplay.entities[i];
        if (entity->entity_type == GAME_ENTITY_NONE) continue;
        Game_Collider *collider = entity_collider(game, entity);
        Game_Vec2 r = {collider->r.x,collider->r.y};   
        game_remap_to_camera(&r, &game->gameplay.cam);
        if (entity->entity_type == GAME_ENTITY_ZOMBIE)
        {
            entity->vel.x = (game->gameplay.player.pos.x-collider->r.x)/1.1;
            entity->vel.y = (game->gameplay.player.pos.y-collider->r.y)/1.1;
            if (entity->vel.x < 0 && entity->vel.x > entity->vel.y) entity->dir = 1; 
            if (entity->vel.x > 0 && entity->vel.x > entity->vel.y) entity->dir = 3; 
            if (entity->vel.y < 0 && entity->vel.y > entity->vel.x) entity->dir = 0; 
            if (entity->vel.y > 0 && entity->vel.y > entity->vel.x) entity->dir = 2; 
            entity->state = ENTITY_STATE_WALK;
            
            Game_Vec2 vp = {collider->r.x, collider->r.y};
            game_remap_to_camera(&vp, &game->gameplay.cam);
            if (vp.x < 0) continue;
            if (vp.y < 0) continue;
            if (vp.x > game_screen.width) continue;
            if (vp.y > game_screen.width) continue;

            for (int i = 0; i < (game->gameplay.colliders.size); i++)
            {
                Game_Collider *srect = vec_index_ptr(game->gameplay.colliders, Game_Collider, i);
                bool col = game_check_collision(srect, collider);
                if (col)
                {
                    if (srect->solid)
                    {
                        Game_Vec2 snap = game_rect_vs_rect_snap(collider->r, srect->r);
                        collider->r.x += snap.x;
                        collider->r.y += snap.y;
                        entity->vel.x = 0;
                        entity->vel.y = 0;
                    }
                    else if (srect->projectile && srect->active)
                    {
                        collider->active = false;
                        srect->active = false;
                        entity->entity_type = GAME_ENTITY_NONE;
                        game->gameplay.entity_count -= 1;
                        game->gameplay.kill_count += 1;
                        break;
                    }
                }
            }

            collider->r.x += entity->vel.x*game->delta;
            collider->r.y += entity->vel.y*game->delta;
            entity->pos.x = collider->r.x-3;
            entity->pos.y = collider->r.y-10;
        }
        else if (entity->entity_type == GAME_ENTITY_PROJECTILE)
        {
            collider->r.x += entity->vel.x*game->delta;
            collider->r.y += entity->vel.y*game->delta;

            if (entity->dir > 1000 || !collider->active)
            {
                collider->active = false;
                entity->entity_type = GAME_ENTITY_NONE;
                game->gameplay.entity_count -= 1;

            }

            // Actually lifespan of entity
            entity->dir += 1000*game->delta;
            entity->pos.x = collider->r.x;
            entity->pos.y = collider->r.y;

        }
    }
}

void game_render_entities(The_Game *game)
{
    for (int i = 0; i < 256; i++)
    {
        Game_Entity *entity = &game->gameplay.entities[i];
        if (entity->entity_type == GAME_ENTITY_NONE) continue;       
        Game_Vec2 r = {entity->pos.x,entity->pos.y};   
        game_remap_to_camera(&r, &game->gameplay.cam);
        Game_Collider *collider = entity_collider(game, entity);

        if (entity->entity_type == GAME_ENTITY_ZOMBIE)
        {
            if (entity->state == ENTITY_STATE_IDLE)
            {
                if (entity->dir == 1)
                {
                    game_screen_blit_rect(&game->textures.tileset, r.x, r.y, 80, 0+16, 16, 16);
                }
                else if (entity->dir == 3)
                {
                    game_screen_blit_rect(&game->textures.tileset, r.x, r.y, 80, 0, 16, 16);
                }
                else if (entity->dir == 0)
                {
                    game_screen_blit_rect(&game->textures.tileset, r.x, r.y, 80, 0+32, 16, 16);
                }
                else if (entity->dir == 2)
                {
                    game_screen_blit_rect(&game->textures.tileset, r.x, r.y, 80, 0+64, 16, 16);
                }
            }
            else
            {       
                if (entity->dir == 1)
                {
                    game_screen_blit_rect(&game->textures.tileset, r.x, r.y, ((int)(game->uptime * 5) % 2) ? 80 : 80+32, 0+16, 16, 16);
                }
                else if (entity->dir == 3)
                {
                    game_screen_blit_rect(&game->textures.tileset, r.x, r.y, ((int)(game->uptime * 5) % 2) ? 80 : 80+32, 0, 16, 16);
                }
                else if (entity->dir == 0)
                {
                    game_screen_blit_rect(&game->textures.tileset, r.x, r.y, 80+((int)(game->uptime * 5) % 3)*16, 0+48, 16, 16);
                }
                else if (entity->dir == 2)
                {
                    game_screen_blit_rect(&game->textures.tileset, r.x, r.y, 80+((int)(game->uptime * 5) % 3)*16, 0+80, 16, 16);
                }  
            }
        }
        else if (entity->entity_type == GAME_ENTITY_PROJECTILE)
        {
            game_screen_blit_rect(&game->textures.tileset, r.x, r.y, 128+((int)(game->uptime * 5) % 4)*8, 0, 8, 8);
        }
       
    }
}
