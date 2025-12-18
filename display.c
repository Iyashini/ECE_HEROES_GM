#include <stdio.h>
#include "display.h"

void display_item(Item item) {
    // Texte coloré en RGB + symbole visible
    printf("\033[38;2;%d;%d;%dm[]\033[0m",
           item.r, item.g, item.b);
}

void display_grid(Grid grid) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            display_item(grid.cells[i][j]);
        }
        printf("\n");
    }
}

void refresh_grid(Grid grid) {
    display_grid(grid);
}
