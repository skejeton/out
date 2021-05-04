#pragma once
#include "s2hooks.h"
#include "bmath.h"
#include "gamestruct.h"


#define PLAYER_STATE_IDLE 0
#define PLAYER_STATE_WALK 1


void game_player_handle(The_Game *game);
void game_player_render(The_Game *game);
