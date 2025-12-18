#ifndef GAME_SYSTEME_H_INCLUDED
#define GAME_SYSTEME_H_INCLUDED
#include <stdbool.h>

// ---------------------------
// Dimensions of the game grid
// ---------------------------
#define ROWS 25
#define COLS 45

// ---------------------------
// Item types
// ---------------------------
typedef enum {
    ITEM_RUBY = 0,
    ITEM_EMERALD,
    ITEM_SAPPHIRE,
    ITEM_TOPAZ,
    ITEM_AMETHYST,
    ITEM_ECE
} ItemType;

// ---------------------------
// Structure for a single item
// ---------------------------
typedef struct {
    int r, g, b;
    ItemType type;
} Item;

// ---------------------------
// Game Grid
// ---------------------------
typedef struct {
    Item cells[ROWS][COLS];
} Grid;

// ---------------------------
// Grid logic
// ---------------------------
Grid grid_generation(int rows, int cols);

#endif
