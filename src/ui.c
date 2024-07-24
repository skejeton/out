#include "ui.h"
#include "texture.h"

int mode;
int justify;
int uix, uiy;
int padding;
int lastcursorindex;
int cursorindex;
int cpx, cpy;
int confirmed;

bool push_box(int *x, int *y, int w, int h)
{
    *x = uix+padding;
    *y = uiy+padding;
    switch (mode) {
        case M_COL:
            switch (justify) {
                case J_LEFT:
                    break;
                case J_MID:
                    *x -= w/2;
                    break;
                case J_RIGHT:
                    *x -= w;
                    break;
            }
            uiy += h+padding*2;
            break;
        case M_ROW:
            switch (justify) {
                case J_LEFT:
                    break;
                case J_MID:
                    *y -= h/2;
                    break;
                case J_RIGHT:
                    *y -= h;
                    break;
            }
            uix += w+padding*2;
            break;
    }

    if (lastcursorindex == cursorindex) {
        cpx = *x-8;
        cpy = *y + h / 2;
        lastcursorindex++;
        return true;
    }

    lastcursorindex++;
    return false;
}

void ui_next()
{
    if (lastcursorindex != 0){
        if (cursorindex == lastcursorindex-1) {
            cursorindex = 0;
            return;
        }
        cursorindex++;
    }
}

void ui_prev()
{
    if (cursorindex == 0 && lastcursorindex != 0) {
        cursorindex = lastcursorindex-1;
        return;
    }
    cursorindex--;
}

void ui_cursorpos(int *x, int *y)
{
    *x = cpx;
    *y = cpy;
}

void ui_reset(bool confirm)
{
    confirmed = confirm;
    if (cursorindex >= lastcursorindex) {
        cursorindex = 0;
    }
    lastcursorindex = 0;
}

void ui_begin(int x, int y, int m, int j) 
{
    uix = x;
    uiy = y;
    mode = m;
    justify = j;
}

void ui_padding(int x)
{
    padding = x;
}

bool ui_button(const char *txt)
{
    int x, y, w, h;
    game_screen_measure(txt, &w, &h);
    bool here = push_box(&x, &y, w, h);
    game_screen_write(txt, x, y);

    if (here) {
        game_screen_rect(x-2, y-2, w+4, h+4, 0xFF007777);
    }

    return here && confirmed;
}