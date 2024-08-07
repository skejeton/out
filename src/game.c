#include "game.h"
#include "bmath.h"
#include "bmp.h"
#include "event.h"
#include "gamestruct.h"
#include "player.h"
#include "room.h"
#include "entity.h"
#include "s2hooks.h"
#include "texture.h"
#include "types.h"
#include "diagnostics.h"
#include <SDL2/SDL_timer.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include <time.h>


u32 ms_uptime() {
    return SDL_GetTicks();
}


void game_stop(The_Game *g);

Event unqueue_wrapped(The_Game *g) {
    Event ev = event_dequeue();
    if (ev.type == EVENT_STOP_PROGRAM) game_stop(g);
    return ev;
}


Event await_wrapped(The_Game *g) {
    Event ev = event_await();
    if (ev.type == EVENT_STOP_PROGRAM) game_stop(g);
    return ev;
}

#define GAME_STATE_MENU 0
#define GAME_STATE_GAMEPLAY 1
#define GAME_STATE_FINISH 2
#define GAME_STATE_FAIL 3

void game_reinit(The_Game *g);

void game_start_at(The_Game *game, int pick)
{
    game_reinit(game);
    game->menu.picked = pick;
    game->time_remaining = game->times[pick];
    game->state = GAME_STATE_GAMEPLAY;
}

int tab = 0;

void game_menu_render(The_Game *game)
{
    game_screen_blit_rect(&game->textures.tileset, 160-(197/2), 20, 0, 128, 197, 32);

    if (tab == 0) {
        ui_begin(320/2, 100, M_COL, J_MID);
        ui_padding(3);
        if (ui_button("Easy")) { game_start_at(game, 0); }
        if (ui_button("Medium")) { game_start_at(game, 1); }
        if (ui_button("Hard")) { game_start_at(game, 2); }
        if (ui_button("Good luck lol")) { game_start_at(game, 3); }
        if (ui_button("More")) { tab++; }
    } else {
        ui_begin(320/2-100, 100, M_ROW, J_LEFT);
        ui_padding(3);
        ui_button("Back");
        ui_button("Statistics");
        ui_button("About");
        ui_button("Settings");
    }

    game_screen_write("Arrow keys, Z - Action 1, X - Action 2", 8, 180);

    int x, y;
    ui_cursorpos(&x, &y);
    game_screen_blit_rect(&game->textures.tileset, x-4, y-4, 128, 8, 8, 8);
    if (game->uptime > 10.0 && !game->menu.used) {
        game_screen_clear(0xFF000000);
        game_screen_write("It seems like you arent doing anything", 8, 50);
        game_screen_write("        If controls dont work         ", 8, 70);
        game_screen_write("Click on the game frame to focus on it", 8, 90);
        game_screen_write("    If it doesnt help, report that    ", 8, 110);
        game_screen_write(" On github issues or in itch comments ", 8, 130);
    }
}

void game_generic_handle(The_Game *game)
{
    Event ev = unqueue_wrapped(game);
    if (ev.type == EVENT_KEYBOARD)
    {
        Keyboard_Event kev = ev.event_data.keyboard;
        switch (convert_scancode_to_char(kev.key))
        {
        case 'r':
            game->state = GAME_STATE_MENU;
            break;
        case 'w':
            game->keys.w = kev.pressed;
            break;
        case 'a':
            game->keys.a = kev.pressed;
            break;
        case 's':
            game->keys.s = kev.pressed;
            break;
        case 'd':
            game->keys.d = kev.pressed;
            break;
        case 'z':
            game->keys.a1 = kev.pressed;
            break;
        case 'x':
            game->keys.a2 = kev.pressed;
            break;
        
        default:
            break;
        }
    } 

}

bool confirmed = false;

void game_menu_handle(The_Game *game)
{
    confirmed = false;
    Event ev = unqueue_wrapped(game);
    if (ev.type == EVENT_KEYBOARD)
    {
        Keyboard_Event kev = ev.event_data.keyboard;
        if (!kev.pressed)
        {
            if (game->uptime > 10.0 && !game->menu.used) {
                game->menu.used = true;              
                return;
            }
            game->menu.used = true;

            if (convert_scancode_to_char(kev.key) == 'w' || convert_scancode_to_char(kev.key) == 'a')
            {
                ui_prev();
            }
            else if (convert_scancode_to_char(kev.key) == 's' || convert_scancode_to_char(kev.key) == 'd')
            {
                ui_next();
            }
            else if (convert_scancode_to_char(kev.key) == 'z' || convert_scancode_to_char(kev.key) == 'x')
            {
                confirmed = true;
            }
        }
    }
}

int snap(double v)
{
    if ((v) < 0) return ((int)v-1);
    return v;
}

void game_render_bg(The_Game *game) {
    game_seed(1);
    for (int i = 0; i < 2000; i += 1) {
        int random_x = game_random()%4000-2000;
        int random_y = game_random()%4000-2000;
        random_x -= game->gameplay.cam.x/1.4;
        random_y -= game->gameplay.cam.y/1.4;
        if (random_x < 16 || random_x > game_screen.width) continue;
        if (random_y < 16 || random_y > game_screen.height) continue;
        int random_tile = (game_random()%7)*16;

        #define INT_MAX 2147483647
        double variation = (double)game_random();

        game_screen_blit_rect(&game->textures.tileset, random_x, random_y+sin((game->uptime)/10.0+variation)*10.0, 0 + random_tile, 240, 16, 16); 
    }
}

void game_gameplay_render(The_Game *game)
{
    game->time_remaining = game->times[game->menu.picked] - (ms_uptime() - game->start_time);
    if (snap(game->gameplay.player.pos.x/16/11) == game->gameplay.exit_room->x &&
        snap(game->gameplay.player.pos.y/16/11) == game->gameplay.exit_room->y)
    {
        game->state = GAME_STATE_FINISH;
    }
    
    
    Vec(Game_Vec2) visited = vec_new(Game_Vec2);
    game_render_rooms(game, &visited, game->gameplay.root_room, 0, 0);
    vec_drop(visited);
    game_render_entities(game);

    char text_buffer[256];

    game_player_render(game);
    sprintf(text_buffer, "%d:%02d", (game->time_remaining/1000)/60, (game->time_remaining/1000)%60);
    game_screen_write(text_buffer, 0, 0);
}


void game_gameplay_handle(The_Game *game)
{
    if (game->time_remaining <= 0) {
        game->state = GAME_STATE_FAIL;
        return;
    }
    game_handle_entities(game);
    game_player_handle(game);    
}


void game_rooms_drop(Game_Room *r, Vec(Game_Vec2) *visited)
{
    if (r == NULL) return;

    for (int i = 0; i < visited->size; i += 1)
    {
        Game_Vec2 el = vec_index(*visited, Game_Vec2, i);
        if (el.x == r->x && el.y == r->y)
        {
            return;
        } 
    }

    vec_push_ref(visited, ((Game_Vec2) { r->x, r->y }));

    if (r)
    {
        game_rooms_drop(r->bottom, visited);
        game_rooms_drop(r->top, visited);
        game_rooms_drop(r->left, visited);
        game_rooms_drop(r->right, visited);

        delete(r);
    }

    return;
}

u32 last_time = 0;

void game_drop(The_Game *g) {
    Vec(Game_Vec2) visited = vec_new(Game_Vec2);
    game_rooms_drop(g->gameplay.root_room, &visited);
    vec_drop(visited);
    vec_drop(g->gameplay.colliders);
    game_texture_drop(g->textures.tileset);
    game_texture_drop(g->textures.mazes);
}

void game_stop(The_Game *g) {
    game_screen_drop();
    game_drop(g);
    exit(0);
}

The_Game game_init(int room_count)
{
    last_time = ms_uptime();
    The_Game g = (The_Game) {
        .state = 0,
        .textures = {
            .tileset = game_read_bmp("ASSETS/TILESET.BMP"),
            .mazes = game_read_bmp("ASSETS/MAZES.BMP")
        },
        .menu = {
            .picked = 0,
            .used = false
        },
        .gameplay = {
            .player = (Game_Player) {
                .pos = { 16*5, 16*5 },
                .vel = { 0, 0 },
                .accel = { 0, 0 },
                .dir = 0,
                .state = 0
            },
            .root_room = NULL,
            .exit_room = NULL,
            .cam = { 0, 0 },
            .colliders = vec_new(Game_Collider),
            .entities = { 0 },
            .entity_count = 0,
            .kill_count = 0
        },
        .keys = {
            .w = false,
            .a = false,
            .s = false,
            .d = false,
            .a1 = false,
            .a2 = false,
        },
        .times = { 180000, 120000, 60000, 3600000/2 },
        .time_remaining = 0,
        .start_time = ms_uptime(),
        .uptime = 0,
        .delta = 0,
        .score = 0
    };
    game_seed(time(NULL));

    for (int i = 0; i < 256; i += 1) {
        Game_Entity entity = {
            .entity_type = GAME_ENTITY_NONE,
            .collider_id = -1,
            .dir = 0,
            .pos = { 0, 0 },
            .state = 0,
            .vel = { 0, 0 }
        };

        g.gameplay.entities[i] = entity;
    }

    
    if (room_count > 0) {
        game_create_rooms(&g, room_count);
        g.gameplay.root_room->maze_type = 0;
        Vec(Game_Vec2) visited = vec_new(Game_Vec2);
        game_create_room_colliders(&g, g.gameplay.root_room, &visited);
        vec_drop(visited);
    }
    
    return g;
}


void game_colliders_render(The_Game *g) {

    vec_foreach(g->gameplay.colliders, i) {
        Game_Collider el = vec_index(g->gameplay.colliders, Game_Collider, i);
        Game_Vec2 position = {el.r.x, el.r.y};   
        game_remap_to_camera(&position, &g->gameplay.cam);
   
        game_screen_rect(position.x, position.y, el.r.w, el.r.h, el.active ? 0x66FF0000 : 0x66FF9999); 
    }
}

void game_reinit(The_Game *g)
{
    game_drop(g);
    int score = g->score;
    int rooms = (g->menu.picked+1)*(g->menu.picked+1)+4;
    switch (g->menu.picked)
    {
    case 0:
        rooms = 12;
        break;
    case 1:
    case 2:
        rooms = 22+game_random()%5;
        break;
    case 3:
        rooms = 100;
        rooms = 400+game_random()%100;
        break;
    default:
        rooms = 1;
        break;
    } 
    *g = game_init(rooms);
    g->score += score;
}

char text_buffer[512];

double delta = 0;
int cscore = 0;

void game_loop(The_Game *g) {
    game_global_diagnostics.fps = 0;
    game_global_diagnostics.skipped_rooms = 0;
    game_global_diagnostics.total_rooms = 0;
    game_screen_clear(0xFF000000);

    game_render_bg(g);
    ui_reset(confirmed);
    switch (g->state)
    {
    case GAME_STATE_MENU:
        sprintf(text_buffer, "Score: %d", g->score);
        game_screen_write(text_buffer, 110, 80);
        game_menu_render(g);
        game_menu_handle(g);
        cscore = 0;
        break;
    case GAME_STATE_GAMEPLAY:
        game_generic_handle(g);
        game_gameplay_handle(g);
        game_gameplay_render(g);
    break;
    case GAME_STATE_FINISH:
        game_screen_write("You did it!", (game_screen.width/2)-(11*8/2), 48);
        cscore += ((double)(g->time_remaining/1000.0))*((double)((g->menu.picked+1)*(g->menu.picked+1)*(g->menu.picked+1)*(g->menu.picked+1)*10));
        sprintf(text_buffer, "Time bonus: %d", cscore);
        game_screen_write(text_buffer, 80, 48+16);


        sprintf(text_buffer, "Kill score: %d", g->gameplay.kill_count * 100*((g->menu.picked+1)*(g->menu.picked+1)*(g->menu.picked+1)));
        game_screen_write(text_buffer, 80, 48+32);


        sprintf(text_buffer, "Total score: %d", cscore);
        game_screen_write(text_buffer, 80, 48+48);

        cscore += g->gameplay.kill_count*100*((g->menu.picked+1)*(g->menu.picked+1)*(g->menu.picked+1));

        g->score += cscore;

        sprintf(text_buffer, "Overall score: %d", g->score);
        game_screen_write(text_buffer, 80, 48+64);

        game_screen_write("Press any key to continue", 80, 150);
        game_screen_paint();
        
        while (unqueue_wrapped(g).type != EVENT_NULL) {}

        
        while (true) {
            Event ev = await_wrapped(g);
            if (ev.type == EVENT_KEYBOARD && ev.event_data.keyboard.pressed) break;
        }
        SDL_Delay(1000);
        g->state = GAME_STATE_MENU;
        while (unqueue_wrapped(g).type != EVENT_NULL) {}
        g->uptime = 0;
        last_time = ms_uptime();

        break;
    case GAME_STATE_FAIL:
        game_screen_write("You lost :c", (game_screen.width/2)-(11*8/2), 48);
        if (g->time_remaining <= 1)
            game_screen_write("  Timeout", (game_screen.width/2)-(11*8/2), 48+8);
        game_screen_write("Press any key to continue", 80, 150);

        game_screen_paint();

        SDL_Delay(1000);
        while (unqueue_wrapped(g).type != EVENT_NULL) {}

        while (true) {
            Event ev = await_wrapped(g);
            if (ev.type == EVENT_KEYBOARD && ev.event_data.keyboard.pressed) break;
        }
        
        SDL_Delay(1000);
        while (unqueue_wrapped(g).type != EVENT_NULL) {}
        g->state = GAME_STATE_MENU;
        g->uptime = 0;
        last_time = ms_uptime();

        break;
    default:
        break;
    }

    delta = ((double)ms_uptime() - (double)last_time)/1000.0;




    g->delta = delta;
    g->uptime += delta;

    last_time = ms_uptime();
    game_screen_paint();

}

#ifdef __EMSCRIPTEN__
void emscripten_loop_wrapper(void *arg) {
    game_loop(arg);
}
#endif

int game_main()
{
    game_screen_init();
    //game_screen_clear(0xFF008080);

    Game_Texture fnt = game_read_bmp("ASSETS/FONT.BMP");
    game_font_init(fnt, "abcdefghijklmnopqrstuvwxyz1234567890!.,?:-", "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!.,?:-");

    /*
    Game_Texture cd = game_read_bmp("ASSETS/CDLOAD.BMP");
    game_screen_write("Loading resources", (game_screen.width/2)-(17*8)/2, (game_screen.height/2)-cd.height/2-16);
    game_screen_blit_rect(&cd, (game_screen.width/2)-cd.width/6, (game_screen.height/2)-cd.height/2, 0, 0, cd.width/3, cd.height);
    game_screen_paint();
    game_texture_drop(cd);*/



    /**
     *  Simulated "loading" wait
     */

    //SDL_Delay(500);

    The_Game *g = new(The_Game, 1);
    *g = game_init(0);



    last_time = ms_uptime();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(emscripten_loop_wrapper, g, 0, 1);
#else
    while (true)
    {
        game_loop(g);
        SDL_Delay(1000/120);
    }
#endif
}
