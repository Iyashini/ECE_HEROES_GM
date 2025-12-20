#include "display.h"
#include <stdio.h>


static void set_bg(int r, int g, int b);
static void set_fg(int r, int g, int b);
static void reset_color(void);



void clear_screen(void) {
    // Efface + curseur en haut (ANSI). Sur Windows 10/11 ça marche si console compatible.
    printf("\033[2J\033[H");
}

void display_menu(int selected) {

    goto_top();

    // ===== TITRE =====
    set_fg(150, 220, 215); // ECE turquoise
    printf("███████╗ ██████╗ ███████╗    ██╗  ██╗███████╗██████╗  ██████╗ ███████╗\n");
    printf("██╔════╝██╔════╝ ██╔════╝    ██║  ██║██╔════╝██╔══██╗██╔═══██╗██╔════╝\n");
    printf("█████╗  ██║      █████╗      ███████║█████╗  ██████╔╝██║   ██║█████╗  \n");
    printf("██╔══╝  ██║      ██╔══╝      ██╔══██║██╔══╝  ██╔══██╗██║   ██║██╔══╝  \n");
    printf("███████╗╚██████╔╝███████╗    ██║  ██║███████╗██║  ██║╚██████╔╝███████╗\n");
    printf("╚══════╝ ╚═════╝ ╚══════╝    ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝\n");
    reset_color();

    printf("\n");

    const char *items[] = {
        "Nouvelle partie",
        "Charger une partie",
        "Regles du jeu",
        "Parametres",
        "Quitter"
    };

    // Couleurs par item (lore)
    int colors[5][3] = {
        {255,160,160}, // Ruby
        {170,230,190}, // Emerald
        {170,200,255}, // Sapphire
        {255,235,170}, // Topaz
        {210,180,255}  // Amethyst
    };

    printf("Utilise HAUT ↑ ou BAS ↓  -  ENTREE pour valider\n\n");

    for (int i = 0; i < 5; i++) {

        if (i == selected) {
            set_bg(colors[i][0]/2, colors[i][1]/2, colors[i][2]/2);
            set_fg(0,0,0);
            printf("  > %-20s  ", items[i]);
            reset_color();
        } else {
            set_fg(colors[i][0], colors[i][1], colors[i][2]);
            printf("    %-20s  ", items[i]);
            reset_color();
        }

        printf("\n");
    }

    printf("\n");
    set_fg(150,220,215);
    printf("✦ Le pouvoir des pierres est entre tes mains ✦\n");
    reset_color();
}


void display_rules(void) {
    clear_screen();

    printf("================ REGLES DU JEU ================\n\n");
    printf("- Objectif : faire des alignements/patterns pour marquer des points.\n");
    printf("- Tu deplaces le curseur, tu selectionnes une case, puis tu swappes.\n");
    printf("- Les patterns valides declenchent suppression + gravite + remplissage.\n");
    printf("- La partie se joue avec un nombre de coups limite ou un temps limite.\n\n");
    printf("Commandes :\n");
    printf("  - Fleches : naviguer\n");
    printf("  - ENTREE  : valider / revenir\n\n");
    printf("==============================================\n");
    printf("Appuie sur ENTREE pour revenir au menu...\n");
}

void display_settings(void) {
    clear_screen();

    printf("================ PARAMETRES ================\n\n");
    printf("Parametres disponibles (version simple) :\n");
    printf("- A venir (difficulte, couleurs, vitesse d'animation, etc.)\n\n");
    printf("===========================================\n");
    printf("Appuie sur ENTREE pour revenir au menu...\n");
}

void display_load_menu(void) {
    clear_screen();

    printf("================ CHARGER UNE PARTIE ================\n\n");
    printf("Chargement (version simple) :\n");
    printf("- A brancher sur ton systeme de sauvegarde (fichier).\n\n");
    printf("====================================================\n");
    printf("Appuie sur ENTREE pour revenir au menu...\n");
}

void display_item(Item item) {
    if (item.type == ITEM_EMPTY) {
        printf("\033[48;2;0;0;0m   \033[0m");
        return;
    }

    printf("\033[48;2;%d;%d;%dm   \033[0m",
           item.r, item.g, item.b);
}

void display_grid(Grid grid) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            display_item(grid.cells[i][j]);
        }
        printf("\n");
    }
}

// ============================
// Affichage de la grille avec curseur & sélection
// ============================
void display_grid_with_cursor(Grid grid, Cursor cursor, Selection sel) {

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {

            Item item = grid.cells[i][j];

            bool is_cursor = (i == cursor.row && j == cursor.col);
            bool is_selected = (sel.active && i == sel.row && j == sel.col);

            // Case sélectionnée (clignotement)
            if (is_selected) {
                if (sel.blink) {
                    printf("\033[48;2;%d;%d;%dm   \033[0m",
                           item.r / 2, item.g / 2, item.b / 2);
                } else {
                    printf("\033[48;2;%d;%d;%dm   \033[0m",
                           item.r, item.g, item.b);
                }
            }
            // Curseur (couleur foncée)
            else if (is_cursor) {
                printf("\033[48;2;%d;%d;%dm   \033[0m",
                       item.r / 2, item.g / 2, item.b / 2);
            }
            // Case normale
            else {
                printf("\033[48;2;%d;%d;%dm   \033[0m",
                       item.r, item.g, item.b);
            }
        }
        printf("\n");
    }
}

void goto_top(void) {
    printf("\033[H");
}

static void set_bg(int r, int g, int b) {
    printf("\033[48;2;%d;%d;%dm", r, g, b);
}

static void set_fg(int r, int g, int b) {
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

static void reset_color(void) {
    printf("\033[0m");
}


