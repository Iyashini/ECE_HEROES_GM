#include <stdio.h>

#include "display.h"

int main(void)
{
    // Génération et affichage d'une grille initiale
    Grid grid = grid_generation(ROWS, COLS);
    display_grid(grid);

    return 0;
}
