#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game_systeme.h"

// ----------------------------------
// Role : Generate and initialize the game grid with random items.
// Prototype : Grid grid_generation(int rows, int cols);
// ----------------------------------
Grid grid_generation(int rows, int cols)
{
    Grid grid;
    int usable_rows = rows > ROWS ? ROWS : rows;
    int usable_cols = cols > COLS ? COLS : cols;

    // Seed the random number generator once
    static int seeded = 0;
    if (!seeded)
    {
        srand((unsigned int) time(NULL));
        seeded = 1;
    }

    // Initialize the grid with random items among the five basic types
    for (int r = 0; r < usable_rows; r++)
    {
        for (int c = 0; c < usable_cols; c++)
        {
            ItemType random_type = (ItemType) (rand() % 5); // Five standard gems
            Item item = {0, 0, 0, random_type};

            // Basic color setup for potential console use
            switch (random_type)
            {
                case ITEM_RUBY:
                    item.r = 255; item.g = 0; item.b = 0;
                    break;
                case ITEM_EMERALD:
                    item.r = 0; item.g = 255; item.b = 0;
                    break;
                case ITEM_SAPPHIRE:
                    item.r = 0; item.g = 0; item.b = 255;
                    break;
                case ITEM_TOPAZ:
                    item.r = 255; item.g = 215; item.b = 0;
                    break;
                case ITEM_AMETHYST:
                    item.r = 153; item.g = 50; item.b = 204;
                    break;
                default:
                    item.r = 255; item.g = 255; item.b = 255;
                    break;
            }

            grid.cells[r][c] = item;
        }
    }

    return grid;
}
