#include "display.h"
#include <stdio.h>

void display_item(Item item) {
    if (item.type == ITEM_EMPTY) {
        printf("\033[48;2;0;0;0m   \033[0m");
        return;
    }

    printf("\033[48;2;%d;%d;%dm   \033[0m",
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

// ============================
// Affichage de la grille avec curseur & sélection
// ============================
void display_grid_with_cursor(Grid grid, Cursor cursor, Selection sel) {

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {

            Item item = grid.cells[i][j];

            bool is_cursor = (i == cursor.row && j == cursor.col);
            bool is_selected = (sel.active && i == sel.row && j == sel.col);

            // Case sélectionnée (clignotement)
            if (is_selected) {
                if (sel.blink) {
                    printf("\033[48;2;%d;%d;%dm   \033[0m",
                           item.r / 2, item.g / 2, item.b / 2);
                } else {
                    printf("\033[48;2;%d;%d;%dm   \033[0m",
                           item.r, item.g, item.b);
                }
            }
            // Curseur (couleur foncée)
            else if (is_cursor) {
                printf("\033[48;2;%d;%d;%dm   \033[0m",
                       item.r / 2, item.g / 2, item.b / 2);
            }
            // Case normale
            else {
                printf("\033[48;2;%d;%d;%dm   \033[0m",
                       item.r, item.g, item.b);
            }
        }
        printf("\n");
    }
}

