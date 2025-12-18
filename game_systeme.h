#ifndef GAME_SYSTEME_H_INCLUDED
#define GAME_SYSTEME_H_INCLUDED
#include <stdbool.h>

// ---------------------------
// Dimensions of the game grid
// ---------------------------
#define ROWS 25
#define COLS 45

// ---------------------------
// Item types
// ---------------------------
typedef enum {
    ITEM_RUBY = 0,
    ITEM_EMERALD,
    ITEM_SAPPHIRE,
    ITEM_TOPAZ,
    ITEM_AMETHYST,
    ITEM_ECE        // Special bonus/malus item
} ItemType;

// ---------------------------
// Structure for a single item
// ---------------------------
typedef struct {
    int r, g, b;     // RGB color
    ItemType type;   // Kind of item
} Item;

// ---------------------------
// Game Grid
// ---------------------------
typedef struct {
    Item cells[ROWS][COLS];
} Grid;

// ---------------------------
// Level objective
// ---------------------------
typedef enum {
    OBJECTIVE_SCORE,
    OBJECTIVE_COLLECT,
    OBJECTIVE_SCORE_AND_COLLECT
} ObjectiveType;

typedef struct {
    ObjectiveType type;
    int target_score;
    ItemType target_item;
    int target_quantity;
    int max_moves;
    int max_time;
} Objective;

// ---------------------------
// Level structure
// ---------------------------
typedef struct {
    int id;
    Objective objective;
} Level;

// ---------------------------
// Game session structure
// ---------------------------
typedef struct {
    char username[50];
    int total_score;
    int lives;
    int global_time;
    bool running;

    Level current_level;
    Grid grid;
} Game;

// ---------------------------
// System function prototypes
// ---------------------------

// Menu logic
void display_choice(int choice);

// Contract logic
char* contract_management(int level_id);

// Grid logic
Grid grid_generation(int rows, int cols);
Grid pattern_recognition(Grid grid);
bool actions_left_management(int actions_left);
bool is_time_over(int time_left);
bool is_dead(int hp);
bool is_game_over(bool actions_left, bool time_over, bool dead);

// Level and progression
bool all_levels_done(int level_id);

// Save / Load
void save_game(Grid grid, char* username, int score, int hp,
               int actions_left, int level_id, char* contract, int time_left);

#endif // GAME_SYSTEME_H_INCLUDED
