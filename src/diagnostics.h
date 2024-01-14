#pragma once
#include <stdio.h>
#include "texture.h"

typedef struct {
    int fps;
    int total_rooms;
    int skipped_rooms;
} Diagnostics;

extern Diagnostics game_global_diagnostics;

void diagnostics_draw();