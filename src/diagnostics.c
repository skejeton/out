#include "diagnostics.h"

Diagnostics game_global_diagnostics;

void diagnostics_draw()
{
    char buffer[100];
    const char *fmt = "FPS: %d\n"
                      "Total Rooms: %d\n"
                      "Skipped Rooms: %d\n";

    sprintf(buffer, fmt, game_global_diagnostics.fps, game_global_diagnostics.total_rooms, game_global_diagnostics.skipped_rooms);
    game_screen_write(buffer, 0, 8);
}
