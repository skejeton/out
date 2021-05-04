#include "event.h"
#include "s2hooks.h"
#include "memory.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <ctype.h>
#include <stdio.h>
#include "math.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif 
static void poll_event();

Event event_await()
{
    while (event_queue.size == 0) {
        #ifdef __EMSCRIPTEN__
        SDL_Delay(1);
        #endif
        poll_event();
    }

    return event_dequeue();
}

void event_dispatch(Event ev)
{
    event_enqueue(ev);
}

void event_queue_init()
{
    event_queue.front = 0;
    event_queue.rear = 0;
    event_queue.size = 0;
}

void event_enqueue(Event ev)
{
    Event_Queue_Node *n = (Event_Queue_Node*)mem_alloc(sizeof(Event_Queue_Node));
    n->ev = ev;
    

    if (event_queue.size == 0)
    {
        event_queue.front = n;
        event_queue.rear = n;
    }
    else 
    {
        event_queue.rear->prev = n;
        n->next = event_queue.rear;
        event_queue.rear = n;
    }
    event_queue.size += 1;
}


static void queue_key(char key, bool pressed)
{
    Keyboard_Event pe = { 
        .key = key,
        .pressed = pressed
    };
    Event e;
    e.type = EVENT_KEYBOARD;
    e.time = 0;
    e.event_data.keyboard = pe;
    event_dispatch(e);
}


int scalex = 2;
int scaley = 2;

static void poll_event() {
    SDL_Event event;

    // Events mangement
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                event_enqueue(((Event) {
                    .type = EVENT_STOP_PROGRAM,
                    .time = 0,
                }));
            break;
            case SDL_WINDOWEVENT: {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    #define max(a,b) (((a)>(b))?(a):(b))
                    #define min(a,b) (((a)<(b))?(a):(b))
                    scalex = max(event.window.data1/320, 1);
                    scaley = max(event.window.data2/200, 1);

                    scalex = scaley = min(scalex, scaley);
                }
            }
            break;
            case SDL_KEYDOWN: {
                if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                    queue_key('w', true);
                } else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                    queue_key('s', true);
                } else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                    queue_key('a', true);                   
                } else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    queue_key('d', true);                   
                }
                else
                queue_key(tolower(SDL_GetScancodeName(event.key.keysym.scancode)[0]), true);
            }
            break;
            case SDL_KEYUP: {
                if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                    queue_key('w', false);
                } else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                    queue_key('s', false);
                } else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                    queue_key('a', false);                   
                } else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    queue_key('d', false);                   
                }
                else
                queue_key(tolower(SDL_GetScancodeName(event.key.keysym.scancode)[0]), false);
            }
            break;
        }
    }
}

Event event_dequeue()
{
    poll_event();

    if (event_queue.size > 0)
    {
        if (event_queue.size == 2)
        {
            event_queue.front->prev = event_queue.rear;
        }
        Event_Queue_Node *n = event_queue.front;
        event_queue.front = event_queue.front->prev;
        Event e = n->ev;
        event_queue.size -= 1;
        mem_free(n);
        return e;
    }
    return (Event) { .type = EVENT_NULL, .time = 0 };
}
