#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#include "game_systeme.h"
#include "display.h"

// ============================
// Redessin complet (sans flash)
// ============================
void redraw_screen(const char *title, Grid grid, Cursor cursor, Selection sel) {
    printf("\033[H");          // curseur en haut à gauche
    printf("%s\n\n", title);   // titre
    display_grid_with_cursor(grid, cursor, sel);
}

int main(void) {

    srand((unsigned int)time(NULL));

    // ============================
    // Initialisation
    // ============================
    Grid grid = grid_generation(ROWS, COLS);

    Cursor cursor = {0, 0};
    Selection sel = {false, 0, 0, false};

    redraw_screen("=== ECE HEROES ===", grid, cursor, sel);

    // ============================
    // Boucle principale
    // ============================
    while (1) {

        // Clignotement sélection
        if (sel.active) {
            sel.blink = !sel.blink;
        }

        redraw_screen("=== ECE HEROES ===", grid, cursor, sel);
        Sleep(120);

        if (!_kbhit())
            continue;

        int key = _getch();

        // ============================
        // Touches étendues (flèches)
        // ============================
        if (key == 0 || key == 224) {
            int arrow = _getch();

            int dr = 0, dc = 0;

            if (arrow == 72) dr = -1; // haut
            if (arrow == 80) dr = 1;  // bas
            if (arrow == 75) dc = -1; // gauche
            if (arrow == 77) dc = 1;  // droite

            // Curseur libre
            if (!sel.active) {
                cursor_move(&cursor, dr, dc);
            }
            // Swap si sélection active
            else {
                if (try_swap(&grid, &sel, dr, dc)) {

                    // Enchaînement moteur classique
                    while (1) {

                        // Suppression
                        grid = remove_marked_items(grid);

                        // Gravité
                        while (gravity_step(&grid)) {
                            redraw_screen("=== GRAVITE ===", grid, cursor, sel);
                            Sleep(50);
                        }

                        // Remplissage
                        if (!has_empty_cells(grid))
                            break;

                        grid = refill_grid(grid);

                        while (gravity_step(&grid)) {
                            redraw_screen("=== REMPLISSAGE ===", grid, cursor, sel);
                            Sleep(50);
                        }
                    }
                }
            }
        }

        // ============================
        // Sélection (ENTER)
        // ============================
        else if (key == 13) { // ENTER
            selection_start(&sel, cursor);
            sel.blink = true;
        }


        // ============================
        // Quitter (Q)
        // ============================
        else if (key == 'q' || key == 'Q') {
            break;
        }
    }

    return 0;
}
