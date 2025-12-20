#ifndef GAME_SYSTEME_H_INCLUDED
#define GAME_SYSTEME_H_INCLUDED

#include <stdbool.h>

#define ROWS 25
#define COLS 45

typedef enum {
    ITEM_EMPTY = -1,
    ITEM_RUBY = 0,
    ITEM_EMERALD,
    ITEM_SAPPHIRE,
    ITEM_TOPAZ,
    ITEM_AMETHYST,
    ITEM_ECE,
    ITEM_BOMB
} ItemType;

typedef struct {
    int r, g, b;
    ItemType type;
} Item;

typedef struct {
    Item cells[ROWS][COLS];
} Grid;

// Curseur + sélection (pour le swap)
typedef struct {
    int row;
    int col;
} Cursor;

typedef struct {
    bool active;
    int row;
    int col;
    bool blink;
} Selection;

typedef struct {
    int score;
    int lives;
    int time_left;   // en secondes
} GameState;


// Génération / patterns
Grid grid_generation(int rows, int cols);
bool pattern_recognition(Grid *grid);

// Suppression / refill / gravité
Grid remove_marked_items(Grid grid);
Grid refill_grid(Grid grid);
bool gravity_step(Grid *grid);
bool has_empty_cells(Grid grid);

// Déplacements / swap
void cursor_move(Cursor *cursor, int dr, int dc);
void selection_start(Selection *sel, Cursor cursor);
bool try_swap(Grid *grid, Selection *sel, int dr, int dc);

// Sauvegarde / Chargement
bool user_exists(const char *pseudo);
void register_user(const char *pseudo);

bool save_game_for_user(const char *pseudo,
                        Grid grid,
                        Cursor cursor,
                        Selection sel,
                        GameState state);

bool load_game_for_user(const char *pseudo,
                        Grid *grid,
                        Cursor *cursor,
                        Selection *sel,
                        GameState *state);


// Liste des utilisateurs
int load_users(char users[][32], int max_users);

void init_game_state(GameState *state);
void update_score(GameState *state, int destroyed);
void lose_life(GameState *state);
bool is_game_over(GameState *state);


#endif
