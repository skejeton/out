#pragma once
#include "s2hooks.h"
#include "texture.h"
#include "bmath.h"

#define GAME_ENTITY_ZOMBIE 0
#define GAME_ENTITY_PROJECTILE 1
#define GAME_ENTITY_NONE 2

typedef struct
{
    Game_Vec2 pos;
    Game_Vec2 vel;
    Game_Vec2 accel;
    int dir;
    int state;
} Game_Player;

typedef struct
{
    Game_Vec2 pos;
    Game_Vec2 vel;
    int dir;
    int entity_type;
    int state;
    int collider_id;
} Game_Entity;

typedef struct Game_Room_T
{
    struct Game_Room_T *top;
    struct Game_Room_T *bottom;
    struct Game_Room_T *left;
    struct Game_Room_T *right;
    int maze_type;
    int x;
    int y;
    int zombies;
} Game_Room;

typedef struct
{
    int state;

    struct
    {
        Game_Texture tileset;
        Game_Texture mazes;
    } textures;
    struct
    {
        u32 picked;
        // This is to indicate inactivity in menu
        bool used;
    } menu;
    struct // Scene
    {
        Game_Player player;
        Game_Room *root_room;
        Game_Room *exit_room;
        Game_Camera cam;
        Vec(GCollider) colliders;
        Game_Entity entities[256];
        int entity_count;
        int kill_count;
    } gameplay;
    struct
    {
        bool w;
        bool a;
        bool s;
        bool d;
        bool a1;
        bool a2;
    } keys;
    
    u32 times[4];
    i32 time_remaining;
    double uptime;
    double start_time;
    double fire_tick;
    double delta;
    int score;
} The_Game;
