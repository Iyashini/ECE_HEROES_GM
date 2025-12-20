#include "game_systeme.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
        default:            item.r = 0;   item.g = 0;   item.b = 0;   break;
    }
    return item;
}

// ============================
// État du jeu
// ============================
void init_game_state(GameState *state) {
    state->score = 0;
    state->lives = 5;
    state->time_left = 300;
}

// ============================
// USERS
// ============================
#define USERS_FILE "users.txt"

bool user_exists(const char *pseudo) {
    FILE *f = fopen(USERS_FILE, "r");
    if (!f) return false;

    char line[64];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, pseudo) == 0) {
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

void register_user(const char *pseudo) {
    // Append user to file
    FILE *f = fopen(USERS_FILE, "a");
    if (!f) return;
    fprintf(f, "%s\n", pseudo);
    fclose(f);
}

int load_users(char users[][32], int max_users) {
    // Load users from file
    FILE *f = fopen(USERS_FILE, "r");
    if (!f) return 0;

    int count = 0;
    while (count < max_users && fgets(users[count], 32, f)) { // limit to 31 chars + null
        users[count][strcspn(users[count], "\n")] = 0;
        count++;
    }
    fclose(f);
    return count;
}

// ============================
// SAVE / LOAD
// ============================
static void make_save_filename(char *out, size_t size, const char *pseudo) {
    snprintf(out, size, "save_%s.txt", pseudo);
}

static const char *SAVE_HEADER = "ECEHEROES_SAVE_V1";

bool save_game_for_user(const char *pseudo, Grid grid, Cursor cursor, Selection sel, GameState state){

    char filename[128];
    make_save_filename(filename, sizeof(filename), pseudo);

    FILE *f = fopen(filename, "w");
    if (!f) return false;

    fprintf(f, "%s\n", SAVE_HEADER);
    fprintf(f, "%d %d\n", ROWS, COLS);
    fprintf(f, "CURSOR %d %d\n", cursor.row, cursor.col);
    fprintf(f, "SELECTION %d %d %d %d\n",
            sel.active ? 1 : 0,
            sel.row,
            sel.col,
            sel.blink ? 1 : 0);

    fprintf(f, "STATE %d %d %d\n",
        state.score,
        state.lives,
        state.time_left);



    fprintf(f, "GRID\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fprintf(f, "%d ", grid.cells[i][j].type);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return true;
}

bool load_game_for_user(const char *pseudo, Grid *grid, Cursor *cursor, Selection *sel, GameState *state){

    char filename[128];
    make_save_filename(filename, sizeof(filename), pseudo);

    FILE *f = fopen(filename, "r");
    if (!f) return false;

    char header[64];
    fgets(header, sizeof(header), f);
    header[strcspn(header, "\n")] = 0;
    if (strcmp(header, SAVE_HEADER) != 0) {
        fclose(f);
        return false;
    }

    int r, c;
    fscanf(f, "%d %d\n", &r, &c);
    fscanf(f, "CURSOR %d %d\n", &cursor->row, &cursor->col);

    int active_i, blink_i;
    fscanf(f, "SELECTION %d %d %d %d\n",
           &active_i, &sel->row, &sel->col, &blink_i);
    sel->active = active_i;
    sel->blink = blink_i;

    if (fscanf(f, "STATE %d %d %d\n",
           &state->score,
           &state->lives,
           &state->time_left) != 3) {
    fclose(f);
    perror("Erreur lecture state");
    return false;
}

    

    char tag[16];
    fgets(tag, sizeof(tag), f);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int t;
            fscanf(f, "%d", &t);
            grid->cells[i][j] = create_item((ItemType)t);
        }
    }

    fclose(f);
    return true;
}

// ============================
// Génération stable
// ============================
Grid grid_generation(int rows, int cols) {
    Grid grid;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            ItemType t;
            int tries = 0;

            do {
                t = rand() % 5;
                tries++;
            } while (
                tries < 20 &&
                ((j >= 2 &&
                  grid.cells[i][j-1].type == t &&
                  grid.cells[i][j-2].type == t) ||
                 (i >= 2 &&
                  grid.cells[i-1][j].type == t &&
                  grid.cells[i-2][j].type == t))
            );

            grid.cells[i][j] = create_item(t);
        }
    }
    return grid;
}

// ============================
// Pattern recognition (>=3)
// ============================
bool pattern_recognition(Grid *grid) {

    bool found = false;
    int marks[ROWS][COLS] = {0};

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j <= COLS - 3; j++) {
            ItemType t = grid->cells[i][j].type;
            if (t != ITEM_EMPTY &&
                grid->cells[i][j+1].type == t &&
                grid->cells[i][j+2].type == t) {

                marks[i][j] = marks[i][j+1] = marks[i][j+2] = 1;
                found = true;
            }
        }
    }

    for (int j = 0; j < COLS; j++) {
        for (int i = 0; i <= ROWS - 3; i++) {
            ItemType t = grid->cells[i][j].type;
            if (t != ITEM_EMPTY &&
                grid->cells[i+1][j].type == t &&
                grid->cells[i+2][j].type == t) {

                marks[i][j] = marks[i+1][j] = marks[i+2][j] = 1;
                found = true;
            }
        }
    }

    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (marks[i][j])
                grid->cells[i][j] = create_item(ITEM_EMPTY);

    return found;
}

// ============================
// Gravité
// ============================
bool gravity_step(Grid *grid) {
    bool moved = false;

    for (int i = ROWS - 2; i >= 0; i--) {
        for (int j = 0; j < COLS; j++) {
            if (grid->cells[i][j].type != ITEM_EMPTY &&
                grid->cells[i+1][j].type == ITEM_EMPTY) {

                grid->cells[i+1][j] = grid->cells[i][j];
                grid->cells[i][j] = create_item(ITEM_EMPTY);
                moved = true;
            }
        }
    }
    return moved;
}

// ============================
// Remplissage
// ============================
Grid refill_grid(Grid grid) {
    for (int j = 0; j < COLS; j++)
        for (int i = 0; i < ROWS; i++)
            if (grid.cells[i][j].type == ITEM_EMPTY) {
                grid.cells[i][j] = create_item(rand() % 5);
                break;
            }
    return grid;
}

bool has_empty_cells(Grid grid) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (grid.cells[i][j].type == ITEM_EMPTY)
                return true;
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

// ============================
// Résolution complète (MOTEUR)
// ============================
int resolve_grid(Grid *grid) {

    int destroyed = 0;
    bool again;

    do {
        again = false;

        if (pattern_recognition(grid)) {
            destroyed++;
            again = true;
        }

        while (gravity_step(grid)) {}

        if (has_empty_cells(*grid)) {
            *grid = refill_grid(*grid);
            again = true;
        }

        while (gravity_step(grid)) {}

    } while (again);

    return destroyed;
}

// ============================
// Swap joueur
// ============================
bool try_swap(Grid *grid, Selection *sel, int dr, int dc) {

    if (!sel->active)
        return false;

    int r1 = sel->row;
    int c1 = sel->col;
    int r2 = r1 + dr;
    int c2 = c1 + dc;

    sel->active = false;

    if (r2 < 0 || r2 >= ROWS || c2 < 0 || c2 >= COLS)
        return false;

    swap_items(grid, r1, c1, r2, c2);

    if (!pattern_recognition(grid)) {
        swap_items(grid, r1, c1, r2, c2);
        return false;
    }

    resolve_grid(grid);
    return true;
}
