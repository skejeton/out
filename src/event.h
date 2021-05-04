#pragma once
#include "ps2.h"
#define EVENT_KEYBOARD 0
#define EVENT_STOP_PROGRAM 1
#define EVENT_NULL 4

#define EVENT_QUEUE_MAX 128

#include "types.h"

extern int scalex;
extern int scaley;

typedef u32 Event_Type;

typedef struct
{
    Event_Type type;
    Time time;
    union {
        Keyboard_Event keyboard;
    } event_data;
} Event;

typedef struct Event_Queue_Node_T
{
    struct Event_Queue_Node_T *next, *prev;
    Event ev;
} Event_Queue_Node;


typedef struct
{
    Event_Queue_Node *rear, *front;
    Size size;
} EventQueue;


static EventQueue event_queue;

void event_queue_init();

void event_enqueue(Event ev);

Event event_dequeue();

Event event_await();

void event_dispatch(Event ev);

Event event_dequeue();
