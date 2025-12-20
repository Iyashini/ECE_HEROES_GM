#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

#include "game_systeme.h"
#include "display.h"

#define MAX_PSEUDO 32

// ============================
// Menu principal
// ============================
int menu_loop(void) {

    int selected = 0;
    const int MENU_COUNT = 5;

    while (1) {

        display_menu(selected);

        int key = _getch();

        if (key == 0 || key == 224) {
            int arrow = _getch();

            if (arrow == 72)       selected = (selected - 1 + MENU_COUNT) % MENU_COUNT;
            else if (arrow == 80)  selected = (selected + 1) % MENU_COUNT;
        }
        else if (key == 13) { // ENTER
            return selected;
        }
        else if (key == 27) { // ESC
            return 4;
        }
    }
}

// ============================
// Boucle de jeu
// ============================
void game_loop(const char *pseudo, Grid *grid, Cursor *cursor, Selection *sel, GameState *state) {

    clear_screen(); // UN seul clear au début

    time_t last_tick = time(NULL);


    while (1) {

        // clignotement sélection
        if (sel->active)
            sel->blink = !sel->blink;

        goto_top();
        goto_top();
        printf("=== ECE HEROES === | Joueur : %s\n", pseudo);
        printf("Score : %d   Vies : %d   Temps : %02d:%02d\n",
            state->score,
            state->lives,
            state->time_left / 60,
            state->time_left % 60);
        printf("[Fleches] Deplacer  [ENTREE] Selection  [S] Sauver  [Q] Menu\n\n");
        display_grid_with_cursor(*grid, *cursor, *sel);

        Sleep(40);

        time_t now = time(NULL);
        if (now - last_tick >= 1) {
            state->time_left--;
            last_tick = now;

            if (state->time_left <= 0) {
                state->lives--;
                state->time_left = 300; // reset timer (5 min)

                if (state->lives <= 0) {
                    goto_top();
                    printf("GAME OVER\n");
                    Sleep(1500);
                    break;
                }
            }
        }


        if (!_kbhit())
            continue;

        int key = _getch();

        // quitter vers menu
        if (key == 'q' || key == 'Q') {
            break;
        }

        // sauvegarde
        if (key == 's' || key == 'S') {

            save_game_for_user(pseudo, *grid, *cursor, *sel, *state);


            goto_top();
            printf("=== ECE HEROES === | Joueur : %s\n", pseudo);
            printf("Sauvegarde OK ! (appuie sur une touche)\n\n");
            _getch();
            continue;
        }

        // flèches directionnelles
        if (key == 0 || key == 224) {

            int arrow = _getch();
            int dr = 0, dc = 0;

            if (arrow == 72) dr = -1;
            if (arrow == 80) dr =  1;
            if (arrow == 75) dc = -1;
            if (arrow == 77) dc =  1;

            if (!sel->active) {
                cursor_move(cursor, dr, dc);
            }
            else {
                if (try_swap(grid, sel, dr, dc)) {
                    state->score += 10;
                    // animation douce pendant la résolution
                    while (gravity_step(grid)) {
                        goto_top();
                        printf("=== ECE HEROES === | Joueur : %s\n", pseudo);
                        printf("[Fleches] Deplacer  [ENTREE] Selection  [S] Sauver  [Q] Menu\n\n");
                        display_grid_with_cursor(*grid, *cursor, *sel);
                        Sleep(15);
                    }
                }
            }
        }

        // sélection
        else if (key == 13) { // ENTER
            selection_start(sel, *cursor);
            sel->blink = 1;
        }
    }

    clear_screen(); // nettoyage avant retour menu
}

// ============================
// MAIN
// ============================
int main(void) {

    clear_screen();
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    srand((unsigned int)time(NULL));

    char pseudo[MAX_PSEUDO];

    while (1) {

        int choice = menu_loop();

        switch (choice) {

            case 0: { // Nouvelle partie
                clear_screen();
                printf("Entrez votre pseudo : ");
                fgets(pseudo, MAX_PSEUDO, stdin);
                pseudo[strcspn(pseudo, "\n")] = 0;

                if (strlen(pseudo) == 0) {
                    printf("Pseudo invalide.\n");
                    Sleep(800);
                    break;
                }

                if (!user_exists(pseudo)) {
                    register_user(pseudo);
                }

                Grid grid = grid_generation(ROWS, COLS);
                Cursor cursor = {0, 0};
                Selection sel = {0, 0, 0, 0};
                GameState state;
                init_game_state(&state);
                game_loop(pseudo, &grid, &cursor, &sel, &state);
                break;
            }

            case 1: { // Charger une partie

                char users[32][32];
                int count = load_users(users, 32);

                if (count <= 0) {
                    clear_screen();
                    printf("Aucune sauvegarde disponible.\n");
                    Sleep(1000);
                    break;
                }

                int selected = 0;

                while (1) {
                    clear_screen();
                    printf("=== CHARGER UNE PARTIE ===\n\n");

                    for (int i = 0; i < count; i++) {
                        if (i == selected) printf(" > %s\n", users[i]);
                        else               printf("   %s\n", users[i]);
                    }

                    printf("\nENTREE = charger | ESC = retour\n");

                    int key = _getch();

                    if (key == 0 || key == 224) {
                        int arrow = _getch();
                        if (arrow == 72) selected = (selected - 1 + count) % count;
                        if (arrow == 80) selected = (selected + 1) % count;
                    }
                    else if (key == 13) {
                        Grid grid;
                        Cursor cursor;
                        Selection sel;

                        GameState state;

                        if (load_game_for_user(users[selected], &grid, &cursor, &sel, &state)) {
                            game_loop(users[selected], &grid, &cursor, &sel, &state);
                        } else {
                            clear_screen();
                            printf("Impossible de charger la partie.\n");
                            Sleep(1000);
                        }
                        break;
                    }
                    else if (key == 27) {
                        break;
                    }
                }
                break;
            }

            case 2:
                display_rules();
                while (_getch() != 13);
                break;

            case 3:
                display_settings();
                while (_getch() != 13);
                break;

            case 4:
                clear_screen();
                printf("Merci d'avoir joue a ECE HEROES.\n");
                Sleep(500);
                return 0;
        }
    }
}
