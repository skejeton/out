#include "types.h"
enum { M_COL, M_ROW };

enum { J_LEFT, J_MID, J_RIGHT };

void ui_next();
void ui_prev();
void ui_cursorpos(int *x, int *y);
void ui_begin(int x, int y, int m, int j);
void ui_padding(int x);
void ui_reset(bool confirm);
bool ui_button(const char *txt);