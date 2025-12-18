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
            item.r = 225; item.g = 160;  item.b = 160;
            break;
        case ITEM_EMERALD:
            item.r = 170;   item.g = 230; item.b = 190;
            break;
        case ITEM_SAPPHIRE:
            item.r = 170;  item.g = 200; item.b = 255;
            break;
        case ITEM_TOPAZ:
            item.r = 255; item.g = 235; item.b = 170;
            break;
        case ITEM_AMETHYST:
            item.r = 210; item.g = 180;  item.b = 255;
            break;
        default:
            item.r = 150; item.g = 220; item.b = 215;
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
