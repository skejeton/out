#pragma once
#include "types.h"

typedef struct
{
  bool pressed;
  Byte key;
} Keyboard_Event;


void keyboard_handler();
char convert_scancode_to_char(Byte code);
