#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>

#include "game_systeme.h"
#include "display.h"

void redraw_screen(const char *title, Grid grid) {
    printf("\033[H");          // curseur en haut
    printf("%s\n\n", title);   // titre fixe
    display_grid(grid);        // grille toujours au même endroit
}

int main(void) {

    srand((unsigned int)time(NULL));

    Grid grid = grid_generation(ROWS, COLS);

    redraw_screen("\n=== GRILLE INITIALE ===\n", grid);
    Sleep(500);

    while (1) {

        // 1. Reconnaissance
        bool found = pattern_recognition(&grid);

        if (!found) {
            redraw_screen("\n=== PLUS AUCUN PATTERN ===\n", grid);
            break; // FIN PROPRE
        }

        // 2. Suppression
        grid = remove_marked_items(grid);

        // 3. Gravité des blocs existants
        while (gravity_step(&grid)) {
            redraw_screen("\n=== GRAVITE EN COURS ===\n", grid);
            Sleep(50);
        }

        // 4. Remplissage + chute des nouveaux blocs
        while (has_empty_cells(grid)) {

            grid = refill_grid(grid);

            while (gravity_step(&grid)) {
                redraw_screen("\n=== REMPLISSAGE ===\n", grid);
                Sleep(30);
            }
        }

        // 5. Grille stable
        redraw_screen("\n=== GRILLE STABLE ===\n", grid);
        Sleep(300);
    }

    return 0;
}
