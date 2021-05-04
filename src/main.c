#include "event.h"
#include "s2hooks.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>


#ifdef _WIN32
#include <windows.h>
int WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd
)
#else
int main(int argc, char *argv[])
#endif
{
    TRACE('A');

    printf("Game started\n");
    event_queue_init();
    TRACE('B');

    game_main();
    return 0;
}

