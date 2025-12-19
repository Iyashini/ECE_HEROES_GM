
#include "game_systeme.h"
#include <stdlib.h>
#include <time.h>


// ============================
// Création d'un item (pastel)
// ============================
static Item create_item(ItemType type) {
    Item item;
    item.type = type;

    switch (type) {
        case ITEM_RUBY:     item.r = 255; item.g = 160; item.b = 160; break;
        case ITEM_EMERALD:  item.r = 170; item.g = 230; item.b = 190; break;
        case ITEM_SAPPHIRE: item.r = 170; item.g = 200; item.b = 255; break;
        case ITEM_TOPAZ:    item.r = 255; item.g = 235; item.b = 170; break;
        case ITEM_AMETHYST: item.r = 210; item.g = 180; item.b = 255; break;
        case ITEM_ECE:      item.r = 150; item.g = 220; item.b = 215; break;
        default:            item.r = 0;   item.g = 0;   item.b = 0;   break;
    }
    return item;
}

// ============================
// Génération de la grille
// ============================
Grid grid_generation(int rows, int cols) {
    Grid grid;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            ItemType t;
            int tries = 0;

            do {
                t = (ItemType)(rand() % 5); // 5 gemmes de base
                tries++;

                // sécurité (au cas où)
                if (tries > 10) break;

            } while (
                // Vérification horizontale
                (j >= 2 &&
                 grid.cells[i][j-1].type == t &&
                 grid.cells[i][j-2].type == t)
                ||
                // Vérification verticale
                (i >= 2 &&
                 grid.cells[i-1][j].type == t &&
                 grid.cells[i-2][j].type == t)
            );

            grid.cells[i][j] = create_item(t);
        }
    }

    return grid;
}

// ============================
// Reconnaissance des patterns
// Marque les cases en BLANC
// Retourne true si au moins un pattern est trouvé
// ============================
bool pattern_recognition(Grid *grid) {

    bool found = false;
    int marks[ROWS][COLS] = {0};

    // =========================
    // 1. CARRÉ 4x4
    // =========================
    for (int i = 0; i <= ROWS - 4; i++) {
        for (int j = 0; j <= COLS - 4; j++) {

            ItemType t = grid->cells[i][j].type;
            bool square = true;

            for (int di = 0; di < 4 && square; di++) {
                for (int dj = 0; dj < 4; dj++) {
                    if (grid->cells[i + di][j + dj].type != t) {
                        square = false;
                        break;
                    }
                }
            }

            if (square) {
                found = true;
                for (int di = 0; di < 4; di++) {
                    for (int dj = 0; dj < 4; dj++) {
                        marks[i + di][j + dj] = 1;
                    }
                }
            }
        }
    }

    // =========================
    // 2. CROIX DE 9
    // =========================
    for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLS - 1; j++) {

            ItemType t = grid->cells[i][j].type;

            if (grid->cells[i - 1][j].type == t &&
                grid->cells[i + 1][j].type == t &&
                grid->cells[i][j - 1].type == t &&
                grid->cells[i][j + 1].type == t) {

                found = true;

                // Ligne
                for (int x = 0; x < COLS; x++) {
                    if (grid->cells[i][x].type == t)
                        marks[i][x] = 1;
                }

                // Colonne
                for (int y = 0; y < ROWS; y++) {
                    if (grid->cells[y][j].type == t)
                        marks[y][j] = 1;
                }
            }
        }
    }

    // =========================
    // 3. SUITE DE 6 (HORIZONTALE)
    // =========================
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j <= COLS - 6; j++) {

            ItemType t = grid->cells[i][j].type;
            bool match = true;

            for (int k = 1; k < 6; k++) {
                if (grid->cells[i][j + k].type != t) {
                    match = false;
                    break;
                }
            }

            if (match) {
                found = true;
                for (int x = 0; x < ROWS; x++) {
                    for (int y = 0; y < COLS; y++) {
                        if (grid->cells[x][y].type == t)
                            marks[x][y] = 1;
                    }
                }
            }
        }
    }

    // =========================
    // 3bis. SUITE DE 6 (VERTICALE)
    // =========================
    for (int j = 0; j < COLS; j++) {
        for (int i = 0; i <= ROWS - 6; i++) {

            ItemType t = grid->cells[i][j].type;
            bool match = true;

            for (int k = 1; k < 6; k++) {
                if (grid->cells[i + k][j].type != t) {
                    match = false;
                    break;
                }
            }

            if (match) {
                found = true;
                for (int x = 0; x < ROWS; x++) {
                    for (int y = 0; y < COLS; y++) {
                        if (grid->cells[x][y].type == t)
                            marks[x][y] = 1;
                    }
                }
            }
        }
    }

    // =========================
    // 4. SUITE DE 4 (HORIZONTALE)
    // =========================
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j <= COLS - 4; j++) {

            ItemType t = grid->cells[i][j].type;
            bool match = true;

            for (int k = 1; k < 4; k++) {
                if (grid->cells[i][j + k].type != t) {
                    match = false;
                    break;
                }
            }

            if (match) {
                found = true;
                for (int k = 0; k < 4; k++)
                    marks[i][j + k] = 1;
            }
        }
    }

    // =========================
    // 4bis. SUITE DE 4 (VERTICALE)
    // =========================
    for (int j = 0; j < COLS; j++) {
        for (int i = 0; i <= ROWS - 4; i++) {

            ItemType t = grid->cells[i][j].type;
            bool match = true;

            for (int k = 1; k < 4; k++) {
                if (grid->cells[i + k][j].type != t) {
                    match = false;
                    break;
                }
            }

            if (match) {
                found = true;
                for (int k = 0; k < 4; k++)
                    marks[i + k][j] = 1;
            }
        }
    }

    // =========================
    // APPLICATION VISUELLE (DEBUG)
    // =========================
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (marks[i][j]) {
                grid->cells[i][j].r = 255;
                grid->cells[i][j].g = 255;
                grid->cells[i][j].b = 255;
            }
        }
    }

    return found;
}



// ============================
// Suppression
// ============================
Grid remove_marked_items(Grid grid) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid.cells[i][j].r == 255 &&
                grid.cells[i][j].g == 255 &&
                grid.cells[i][j].b == 255) {

                grid.cells[i][j].type = ITEM_EMPTY;
                grid.cells[i][j].r = 0;
                grid.cells[i][j].g = 0;
                grid.cells[i][j].b = 0;
            }
        }
    }
    return grid;
}

// // ============================
// // Gravité (CORRECTE)
// // ============================
// Grid apply_gravity(Grid grid) {
//     for (int col = 0; col < COLS; col++) {

//         int write_row = ROWS - 1;

//         for (int row = ROWS - 1; row >= 0; row--) {
//             if (grid.cells[row][col].type != ITEM_EMPTY) {
//                 grid.cells[write_row][col] = grid.cells[row][col];
//                 write_row--;
//             }
//         }

//         for (int row = write_row; row >= 0; row--) {
//             grid.cells[row][col].type = ITEM_EMPTY;
//             grid.cells[row][col].r = 0;
//             grid.cells[row][col].g = 0;
//             grid.cells[row][col].b = 0;
//         }
//     }
//     return grid;
// }

// ============================
// Remplissage
// ============================
Grid refill_grid(Grid grid) {
    for (int col = 0; col < COLS; col++) {
        for (int row = 0; row < ROWS; row++) {
            if (grid.cells[row][col].type == ITEM_EMPTY) {
                ItemType t = (ItemType)(rand() % 5);
                grid.cells[row][col] = create_item(t);
                break; // UN nouveau bloc par colonne et par frame
            }
        }
    }
    return grid;
}


bool gravity_step(Grid *grid) {
    bool moved = false;

    // On parcourt de bas en haut
    for (int i = ROWS - 2; i >= 0; i--) {
        for (int j = 0; j < COLS; j++) {

            if (grid->cells[i][j].type != ITEM_EMPTY &&
                grid->cells[i + 1][j].type == ITEM_EMPTY) {

                // On fait "tomber" l'item d'une case
                grid->cells[i + 1][j] = grid->cells[i][j];

                grid->cells[i][j].type = ITEM_EMPTY;
                grid->cells[i][j].r = 0;
                grid->cells[i][j].g = 0;
                grid->cells[i][j].b = 0;

                moved = true;
            }
        }
    }

    return moved;
}


bool has_empty_cells(Grid grid) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid.cells[i][j].type == ITEM_EMPTY)
                return true;
        }
    }
    return false;
}
