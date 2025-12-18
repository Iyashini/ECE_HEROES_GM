#include <stdlib.h>
#include "game_systeme.h"

// ----------------------------------
// Création d'un item à partir de son type
// ----------------------------------
static Item create_item(ItemType type) {
    Item item;
    item.type = type;

    switch (type) {
        case ITEM_RUBY:
            item.r = 220; item.g = 20;  item.b = 60;
            break;
        case ITEM_EMERALD:
            item.r = 0;   item.g = 200; item.b = 80;
            break;
        case ITEM_SAPPHIRE:
            item.r = 30;  item.g = 144; item.b = 255;
            break;
        case ITEM_TOPAZ:
            item.r = 255; item.g = 215; item.b = 0;
            break;
        case ITEM_AMETHYST:
            item.r = 138; item.g = 43;  item.b = 226;
            break;
        default:
            item.r = 255; item.g = 255; item.b = 255;
            break;
    }

    return item;
}

// ----------------------------------
// Génération complète de la grille
// ----------------------------------
Grid grid_generation(int rows, int cols) {
    Grid grid;

    // La grille est de taille fixe (ROWS x COLS)
    (void)rows;
    (void)cols;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            ItemType type = (ItemType)(rand() % 5);
            grid.cells[i][j] = create_item(type);
        }
    }

    return grid;
}
