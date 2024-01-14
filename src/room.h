#pragma once
#include "s2hooks.h"
#include "gamestruct.h"

void game_render_room(The_Game *game, Game_Room *room, int x, int y);

void game_render_rooms(The_Game *game, Vec(Game_Vec2) *visited, Game_Room *cr, int in, int jn);

void game_handle_rooms(The_Game *game);

Game_Room* game_create_room(The_Game *game, int x, int y);

Game_Room* game_create_rooms(The_Game *game, int count);

void game_create_room_colliders(The_Game *game, Game_Room *r, Vec(Game_Vec2) *visited);
