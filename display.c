#include <stdio.h>

#include "display.h"

// --------------------------------------------------
// Role : Obtain RGB values for a given item type.
// Prototype : static void get_item_color(Item item, int *r, int *g, int *b);
// --------------------------------------------------
static void get_item_color(Item item, int *r, int *g, int *b)
{
    switch (item.type)
    {
        case ITEM_RUBY:     *r = 240; *g = 128; *b = 128; break; // pastel red/pink
        case ITEM_EMERALD:  *r = 144; *g = 238; *b = 144; break; // light green
        case ITEM_SAPPHIRE: *r = 173; *g = 216; *b = 230; break; // light blue
        case ITEM_TOPAZ:    *r = 255; *g = 239; *b = 213; break; // light golden
        case ITEM_AMETHYST: *r = 216; *g = 191; *b = 216; break; // light purple
        case ITEM_ECE:      *r = 255; *g = 182; *b = 193; break; // pastel pink
        default:            *r = 211; *g = 211; *b = 211; break; // light gray for unknown
    }
}

// --------------------------------------------------
// Role : Display a single item using a colored background (pastel RGB).
// Prototype : void display_item(Item item);
// --------------------------------------------------
void display_item(Item item)
{
    int r = 0, g = 0, b = 0;
    get_item_color(item, &r, &g, &b);

    // ANSI escape for 24-bit background color, using two spaces for visibility.
    printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", r, g, b);
}

// --------------------------------------------------
// Role : Display the entire game grid on the console.
// Prototype : void display_grid(Grid grid);
// --------------------------------------------------
void display_grid(Grid grid)
{
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            display_item(grid.cells[r][c]);
        }
        printf("\n");
    }
}
