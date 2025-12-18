#include <stdlib.h>
#include <time.h>

#include "game_systeme.h"
#include "display.h"

int main(void) {

    // ============================
    // Initialisation du programme
    // ============================

    // Initialisation du générateur aléatoire (UNE SEULE FOIS)
    srand((unsigned int)time(NULL));

    // ============================
    // Création de la grille
    // ============================

    Grid grid = grid_generation(ROWS, COLS);

    // ============================
    // Affichage de la grille
    // ============================

    display_grid(grid);

    return 0;
}
