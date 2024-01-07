#include <stdio.h>
#include <string.h>
#include "supermorpion.h"

void displayGame(SuperMorpion *game) {
    for (int row = 0; row < 3; row++) {
        if (row > 0) {
            // Ajouter une séparation entre les grandes lignes de grilles
            for (int i = 0; i < 3 * 2 + 2 * 3; i++) { // 3 caractères par cellule, 2 espaces entre les grilles, 3 grilles
                printf("--");
            }
            printf("\n");
        }

        for (int subRow = 0; subRow < 3; subRow++) {
            for (int col = 0; col < 3; col++) {
                if (col > 0) {
                    printf("| "); // Séparateur entre les petites grilles
                }

                for (int subCol = 0; subCol < 3; subCol++) {
                    char cell = game->smallGrids[row][col].grid[subRow][subCol];
                    printf("%c ", cell ? cell : ' ');
                }
            }
            printf("\n");
        }
    }
}

void initializeSuperMorpion(SuperMorpion *game) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Initialiser chaque petite grille
            initializeGrid(&(game->smallGrids[i][j])) ;
            //game->smallGrids[i][j].currentPlayer = 'X'; // Initialiser le premier joueur, par exemple 'X'
        }
    }
}

int inputMove(SuperMorpion *game) {
    char col;
    int grid, row;
    printf("Entrez votre coup (ex: 3 c3): ");
    if (scanf("%d %c%d", &grid, &col, &row) != 3) {
        printf("Erreur de saisie. Réessayez.\n");
        return 0; // Échec de la saisie
    }

    // Conversion en indices de tableau (0-based)
    int gridIndex = grid - 1;
    int rowIndex = 2-(row - 1);
    int colIndex = col - 'a';

    // Validation des indices
    if (gridIndex < 0 || gridIndex >= 9 ||
        rowIndex < 0 || rowIndex >= 3 ||
        colIndex < 0 || colIndex >= 3 ||
        game->smallGrids[gridIndex / 3][gridIndex % 3].grid[rowIndex][colIndex] != ' ') {
        printf("Coup invalide. Réessayez.\n");
        return 0; // Coup invalide
    }

    // Appliquer le coup
    game->smallGrids[gridIndex / 3][gridIndex % 3].grid[rowIndex][colIndex] = game->currentPlayer;

    // Changer le joueur actuel
    game->currentPlayer = (game->currentPlayer == 'x') ? 'o' : 'x';

    return 1; // Succès
}

char* generateSmallGridGraphviz(GameState grid) {
    static char buffer[1024];
    snprintf(buffer, sizeof(buffer), 
        "<TABLE border=\"0\" cellspacing=\"0\" cellpadding=\"4\" style=\"rounded\" bgcolor=\"white\">\n");

    for (int i = 0; i < 3; i++) {
        strcat(buffer, "  <TR>\n");
        for (int j = 0; j < 3; j++) {
            char cell = grid.grid[i][j];
            char cellContent[30];
            sprintf(cellContent, "<TD bgcolor=\"white\">%c</TD>\n", cell ? cell : ' ');
            strcat(buffer, cellContent);
        }
        strcat(buffer, "  </TR>\n");
    }

    strcat(buffer, "</TABLE>\n");
    return buffer;
}

void displaySuperMorpionGraphviz(SuperMorpion *game, FILE *file) {
    fprintf(file, "digraph super_morpion {\n");
    fprintf(file, "  node [shape=none];\n");
    fprintf(file, "  a0 [label=<\n");
    fprintf(file, "  <TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">\n");

    for (int i = 0; i < 3; i++) {
        fprintf(file, "    <TR>\n");
        for (int j = 0; j < 3; j++) {
            fprintf(file, "      <TD>%s</TD>\n", generateSmallGridGraphviz(game->smallGrids[i][j]));
        }
        fprintf(file, "    </TR>\n");
    }

    fprintf(file, "  </TABLE>\n");
    fprintf(file, "  >];\n");
    fprintf(file, "}\n");
}
