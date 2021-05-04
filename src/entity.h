#pragma once
#include "s2hooks.h"
#include "bmath.h"
#include "gamestruct.h"

#define ENTITY_STATE_IDLE 0
#define ENTITY_STATE_WALK 1

void game_add_zombie(The_Game *game, double x, double y);

void game_add_projectile(The_Game *game, double x, double y, double vx, double vy);
void game_render_entities(The_Game *game);
void game_handle_entities(The_Game *game);
