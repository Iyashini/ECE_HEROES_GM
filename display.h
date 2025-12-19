#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED
#include "game_systeme.h"

// Menu & rules
void display_menu();
void display_rules();

// Grid & items
void display_grid(Grid grid);
void display_item(Item item);
void refresh_grid(Grid grid);

// Game information
void display_score(int score);
void display_hp(int hp);
void display_time_left(int time_left);
void display_level_id(int level_id);
void display_level_contract(char* contract);
void display_actions_left(int actions);
void display_issue(char* issue);

// User information
void display_username(char* username);

// Final stats
void display_stats(int score, int hp, int time_left, int actions_left);

// Grid with cursor & selection
void display_grid_with_cursor(Grid grid, Cursor cursor, Selection sel);

#endif // DISPLAY_H_INCLUDED
