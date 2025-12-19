#ifndef GAME_SYSTEME_H_INCLUDED
#define GAME_SYSTEME_H_INCLUDED

#include <stdbool.h>

#define ROWS 25
#define COLS 45

typedef enum {
    ITEM_EMPTY = -1,
    ITEM_RUBY = 0,
    ITEM_EMERALD,
    ITEM_SAPPHIRE,
    ITEM_TOPAZ,
    ITEM_AMETHYST,
    ITEM_ECE
} ItemType;

typedef struct {
    int r, g, b;
    ItemType type;
} Item;

typedef struct {
    Item cells[ROWS][COLS];
} Grid;

// Génération
Grid grid_generation(int rows, int cols);

// Patterns
bool pattern_recognition(Grid *grid);

// Suppression / gravité / remplissage
Grid remove_marked_items(Grid grid);
Grid apply_gravity(Grid grid);
Grid refill_grid(Grid grid);
bool gravity_step(Grid *grid);
void animate_gravity(Grid *grid);
bool has_empty_cells(Grid grid);



#endif
