#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "game_systeme.h"

// ===== ECRAN =====
void clear_screen(void);
void goto_top(void);

// ===== MENU =====
void display_menu(int selected);
void display_rules(void);
void display_settings(void);
void display_load_menu(void);

// ===== GRILLE =====
void display_item(Item item);
void display_grid(Grid grid);
void display_grid_with_cursor(Grid grid, Cursor cursor, Selection sel);

#endif
