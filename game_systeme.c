#include "game_systeme.h"
#include <stdlib.h>

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
// Génération stable de la grille
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
                if (tries > 20) break;
            } while (
                (j >= 2 &&
                 grid.cells[i][j-1].type == t &&
                 grid.cells[i][j-2].type == t)
                ||
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
// Reconnaissance des patterns (marque en blanc)
// Retourne true si au moins un pattern est trouvé
// ============================
bool pattern_recognition(Grid *grid) {

    bool found = false;
    int marks[ROWS][COLS] = {0};

    // =========================
    // 0) CARRÉ 3x3 (9 items)
    // =========================
    for (int i = 0; i <= ROWS - 3; i++) {
        for (int j = 0; j <= COLS - 3; j++) {

            ItemType t = grid->cells[i][j].type;
            if (t == ITEM_EMPTY)
                continue;

            bool square = true;

            for (int di = 0; di < 3 && square; di++) {
                for (int dj = 0; dj < 3; dj++) {
                    if (grid->cells[i + di][j + dj].type != t) {
                        square = false;
                        break;
                    }
                }
            }

            if (square) {
                found = true;
                for (int di = 0; di < 3; di++)
                    for (int dj = 0; dj < 3; dj++)
                        marks[i + di][j + dj] = 1;
            }
        }
    }

    // =========================
    // 1) CARRÉ 4x4
    // =========================
    for (int i = 0; i <= ROWS - 4; i++) {
        for (int j = 0; j <= COLS - 4; j++) {

            ItemType t = grid->cells[i][j].type;
            if (t == ITEM_EMPTY) continue;

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
                for (int di = 0; di < 4; di++)
                    for (int dj = 0; dj < 4; dj++)
                        marks[i + di][j + dj] = 1;
            }
        }
    }

    // =========================
    // 2) CROIX DE 9
    // =========================
    for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLS - 1; j++) {

            ItemType t = grid->cells[i][j].type;
            if (t == ITEM_EMPTY) continue;

            if (grid->cells[i-1][j].type == t &&
                grid->cells[i+1][j].type == t &&
                grid->cells[i][j-1].type == t &&
                grid->cells[i][j+1].type == t) {

                found = true;

                for (int x = 0; x < COLS; x++)
                    if (grid->cells[i][x].type == t)
                        marks[i][x] = 1;

                for (int y = 0; y < ROWS; y++)
                    if (grid->cells[y][j].type == t)
                        marks[y][j] = 1;
            }
        }
    }

    // =========================
    // 3) SUITE DE 6 (H + V)
    // =========================
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j <= COLS - 6; j++) {

            ItemType t = grid->cells[i][j].type;
            if (t == ITEM_EMPTY) continue;

            bool match = true;
            for (int k = 1; k < 6; k++) {
                if (grid->cells[i][j + k].type != t) {
                    match = false;
                    break;
                }
            }

            if (match) {
                found = true;
                for (int x = 0; x < ROWS; x++)
                    for (int y = 0; y < COLS; y++)
                        if (grid->cells[x][y].type == t)
                            marks[x][y] = 1;
            }
        }
    }

    for (int j = 0; j < COLS; j++) {
        for (int i = 0; i <= ROWS - 6; i++) {

            ItemType t = grid->cells[i][j].type;
            if (t == ITEM_EMPTY) continue;

            bool match = true;
            for (int k = 1; k < 6; k++) {
                if (grid->cells[i + k][j].type != t) {
                    match = false;
                    break;
                }
            }

            if (match) {
                found = true;
                for (int x = 0; x < ROWS; x++)
                    for (int y = 0; y < COLS; y++)
                        if (grid->cells[x][y].type == t)
                            marks[x][y] = 1;
            }
        }
    }

    // =========================
    // 4) SUITE DE 4 (H + V)
    // =========================
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j <= COLS - 4; j++) {

            ItemType t = grid->cells[i][j].type;
            if (t == ITEM_EMPTY) continue;

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

    for (int j = 0; j < COLS; j++) {
        for (int i = 0; i <= ROWS - 4; i++) {

            ItemType t = grid->cells[i][j].type;
            if (t == ITEM_EMPTY) continue;

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
    // Application des marques
    // =========================
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (marks[i][j]) {
                grid->cells[i][j].type = ITEM_EMPTY;
            }
        }
    }

    return found;
}


// ============================
// Suppression (blanc -> empty)
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

// ============================
// Remplissage (1 par colonne)
// ============================
Grid refill_grid(Grid grid) {
    for (int col = 0; col < COLS; col++) {
        for (int row = 0; row < ROWS; row++) {
            if (grid.cells[row][col].type == ITEM_EMPTY) {
                ItemType t = (ItemType)(rand() % 5);
                grid.cells[row][col] = create_item(t);
                break;
            }
        }
    }
    return grid;
}

// ============================
// Gravité pas à pas
// ============================
bool gravity_step(Grid *grid) {
    bool moved = false;

    for (int i = ROWS - 2; i >= 0; i--) {
        for (int j = 0; j < COLS; j++) {

            if (grid->cells[i][j].type != ITEM_EMPTY &&
                grid->cells[i + 1][j].type == ITEM_EMPTY) {

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

// ============================
// Curseur / sélection / swap
// ============================
void cursor_move(Cursor *cursor, int dr, int dc) {
    cursor->row = (cursor->row + dr + ROWS) % ROWS;
    cursor->col = (cursor->col + dc + COLS) % COLS;
}

void selection_start(Selection *sel, Cursor cursor) {
    sel->active = true;
    sel->row = cursor.row;
    sel->col = cursor.col;
}

static void swap_items(Grid *grid, int r1, int c1, int r2, int c2) {
    Item tmp = grid->cells[r1][c1];
    grid->cells[r1][c1] = grid->cells[r2][c2];
    grid->cells[r2][c2] = tmp;
}

bool try_swap(Grid *grid, Selection *sel, int dr, int dc) {

    if (!sel->active)
        return false;

    int r2 = sel->row + dr;
    int c2 = sel->col + dc;

    if (r2 < 0 || r2 >= ROWS || c2 < 0 || c2 >= COLS)
        return false;

    swap_items(grid, sel->row, sel->col, r2, c2);

    // Important: pattern_recognition marque en blanc => ça modifie la grille.
    // Pour valider un swap proprement, il faudra une fonction "check_only" plus tard.
    // Pour l'instant, on accepte cette approche debug.
    if (!pattern_recognition(grid)) {
        swap_items(grid, sel->row, sel->col, r2, c2);
        sel->active = false;
        return false;
    }

    sel->active = false;
    return true;
}


