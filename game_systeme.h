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

// Curseur + sélection (pour le swap)
typedef struct {
    int row;
    int col;
} Cursor;

typedef struct {
    bool active;
    int row;
    int col;
    bool blink;
} Selection;

// Génération / patterns
Grid grid_generation(int rows, int cols);
bool pattern_recognition(Grid *grid);

// Suppression / refill / gravité
Grid remove_marked_items(Grid grid);
Grid refill_grid(Grid grid);
bool gravity_step(Grid *grid);
bool has_empty_cells(Grid grid);

// Déplacements / swap
void cursor_move(Cursor *cursor, int dr, int dc);
void selection_start(Selection *sel, Cursor cursor);
bool try_swap(Grid *grid, Selection *sel, int dr, int dc);


#endif
