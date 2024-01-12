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
    printf("\n");
}

void initializeSuperMorpion(SuperMorpion *game) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Initialiser chaque petite grille
            initializeGrid(&(game->smallGrids[i][j])) ;
            game->smallGrids[i][j].winner = ' '; 
        }
    }
    game->lastMoveCol=-1;
    game->lastMoveRow=-1;
    game->currentPlayer='x';
    game->timelimit=900;
    game->start_time=0;
}


int validateMove(SuperMorpion *game, int gridIndex, int rowIndex, int colIndex) {
    // Validation des indices
    if (gridIndex < 0 || gridIndex >= 9 ||
        rowIndex < 0 || rowIndex >= 3 ||
        colIndex < 0 || colIndex >= 3) {
      //  printf("Coup invalide. Réessayez.\n");
        return 0; // Coup invalide
    }
    // Vérifier si le coup est dans la bonne grille
    if ((game->lastMoveRow != -1 && game->lastMoveCol != -1 && 
         gridIndex != game->lastMoveRow * 3 + game->lastMoveCol) && game->smallGrids[game->lastMoveRow][game->lastMoveCol].winner == ' ' ||
        game->smallGrids[gridIndex / 3][gridIndex % 3].winner != ' ') {
  //          printf("%d %d %d winner:%c\n",game->lastMoveRow ,game->lastMoveCol,gridIndex,game->smallGrids[gridIndex / 3][gridIndex % 3].winner);
//        printf("Coup invalide. Réessayez.\n");
        
        return 0; // Coup invalide
    }
    // Vérifier si la case est vide
    if (game->smallGrids[gridIndex / 3][gridIndex % 3].grid[rowIndex][colIndex] != ' ') {
  //      printf("Coup invalide. Réessayez.\n");
        return 0; // Coup invalide
    }
    return 1; // Coup valide
}

int inputMove(SuperMorpion *game) {
    char col;
    int grid, row;
    printf("Entrez votre coup (ex: 3 c3)(dernier coup:ligne:%d colonne:%d): ",game->lastMoveRow+1,game->lastMoveCol+1);
    if (scanf("%d %c%d", &grid, &col, &row) != 3) {
        printf("Erreur de saisie. Réessayez.\n");
        return 0; // Échec de la saisie
    }
    // Conversion en indices de tableau (0-based)
    int gridIndex = grid - 1;
    int rowIndex = 2-(row - 1);
    int colIndex = col - 'a';
    // Validation des indices
            for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
            {   
                updateGridState(&game->smallGrids[i][j]);
            }
    char stat=game->smallGrids[game->lastMoveRow][game->lastMoveCol].winner;
    if(stat=='x' || stat=='o' || stat=='d' ){
        game->lastMoveRow=-1;
        game->lastMoveCol=-1;

    }
    if (!validateMove(game, gridIndex, rowIndex, colIndex)) {
        printf("Coup invalide. Réessayez.\n");
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
            {   
                stat=game->smallGrids[i][j].winner;
        if(stat=='x' || stat=='o' || stat=='d' ){
        printf("vailde coupe:%d %d \n",i,j);
    }
            }           
        return 0; // Coup invalide, ne pas continuer
    }
    // Appliquer le coup
    game->smallGrids[gridIndex / 3][gridIndex % 3].grid[rowIndex][colIndex] = game->currentPlayer;
    game->lastMoveRow=rowIndex;
    game->lastMoveCol=colIndex;
    updateGridState(&game->smallGrids[gridIndex / 3][gridIndex % 3]);
    stat=game->smallGrids[gridIndex/3][gridIndex%3].winner;
    // Changer le joueur actuel
    game->currentPlayer = (game->currentPlayer == 'x') ? 'o' : 'x';
    return 1; // Succès
}

char* generateSmallGridGraphviz(GameState grid) {
    static char buffer[1024];
    snprintf(buffer, sizeof(buffer), 
        "<TABLE border=\"1\" cellspacing=\"0\" cellpadding=\"4\" style=\"rounded\" bgcolor=\"white\">\n");
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
    fprintf(file, "  <TABLE border=\"0\" cellspacing=\"0\" cellpadding=\"0\" style=\"rounded\" bgcolor=\"black\">\n");
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

void updateGridState(GameState *grid) {
    grid->winner=' ';
    // Vérifier les victoires pour 'x' et 'o'
    const char players[2] = {'x', 'o'};
    for (int p = 0; p < 2; p++) {
        char player = players[p];
        // Vérifier les lignes, colonnes et diagonales pour un alignement gagnant
        for (int i = 0; i < 3; i++) {
            if ((grid->grid[i][0] == player && grid->grid[i][1] == player && grid->grid[i][2] == player) ||
                (grid->grid[0][i] == player && grid->grid[1][i] == player && grid->grid[2][i] == player)) {
                grid->winner = player; // Le joueur a gagné
                return;
            }
        }
        if ((grid->grid[0][0] == player && grid->grid[1][1] == player && grid->grid[2][2] == player) ||
            (grid->grid[0][2] == player && grid->grid[1][1] == player && grid->grid[2][0] == player)) {
            grid->winner = player; // Le joueur a gagné
            return;
        }
    }
    // Vérifier si la grille est complète sans gagnant
    int isFull = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid->grid[i][j] == ' ') {
                isFull = 0;
                break;
            }
        }
        if (!isFull) break;
    }
    if (isFull && grid->winner == ' ') {
        grid->winner = 'd'; // Marquez comme match nul
    }
}

void superparseFEN(SuperMorpion *game, const char *fen) {
    int fenIndex = 0;
    while(fen[fenIndex]=='\'' || fen[fenIndex]=='\"')fenIndex++;
    // Lire les 9 chaînes FEN pour chaque petite grille
    for (int i = 0; i < 9; i++) {
        GameState *grid = &game->smallGrids[i / 3][i % 3];
        grid->winner = ' '; // Initialiser le gagnant de la grille comme vide
        for (int j = 0; j < 9; j++, fenIndex++) {
            char c = fen[fenIndex];
            if (c >= '1' && c <= '9') {
                // Remplir les cases vides
                int emptyCount = c - '0';
                for (int k = 0; k < emptyCount; k++) {
                    grid->grid[j / 3][j % 3] = ' ';
                    if (k < emptyCount - 1) j++;
                }
            } else {
                // Remplir avec 'x', 'o', ou marquer le gagnant de la grille
                if (c == 'X') {
                    grid->winner = 'x'; // La grille est gagnée par 'x'
                    parseFEN("xxxxxxxxx x",grid);
                    fenIndex++;
                    break;
                } else if (c == 'O') {
                    grid->winner = 'o'; // La grille est gagnée par 'o'
                    parseFEN("ooooooooo o",grid);
                    fenIndex++;
                    break;
                } else {
                    grid->grid[j / 3][j % 3] = c;
                }
            }
        }
    }
    // Analyser le dernier coup joué et le joueur au trait
    int lastMoveGrid,lastMoveCase;
    lastMoveGrid=fen[fenIndex+1]-'0';
    lastMoveCase=fen[fenIndex+2]-'0'-1;
    game->currentPlayer=fen[fenIndex+4];
    game->lastMoveCol=lastMoveCase%3;
    game->lastMoveRow=lastMoveCase/3;
    
    return ;
}

int isGridPlayable(GameState *grid) {
    if (grid->winner != ' ') return 0; // La grille a un gagnant
    // Vérifier si la grille est complète
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid->grid[i][j] == ' ') return 1; // Il y a de l'espace pour jouer
        }
    }
    return 0; // La grille est complète
}

int playMove(SuperMorpion *game, int gridIndex, int rowIndex, int colIndex) {
    // Valider le coup
    if (!validateMove(game, gridIndex, rowIndex, colIndex)) {
        return 0; // Coup invalide, ne pas continuer
    }
    // Coup valide, jouer le coup
    GameState *targetGrid = &game->smallGrids[gridIndex / 3][gridIndex % 3];
    targetGrid->grid[rowIndex][colIndex] = game->currentPlayer;
    // Mettre à jour l'état de la petite grille
    updateGridState(targetGrid);
    // Changer de joueur
    game->currentPlayer = (game->currentPlayer == 'x') ? 'o' : 'x';
    // Mettre à jour lastMoveRow et lastMoveCol pour la prochaine grille cible
    game->lastMoveRow = rowIndex;
    game->lastMoveCol = colIndex;
    return 1;
}

int evaluateGameState(SuperMorpion *game) {
    // Vérifier les lignes et les colonnes pour la victoire
    for (int i = 0; i < 3; i++) {
        // Vérifier les lignes
        if (game->smallGrids[i][0].winner == game->smallGrids[i][1].winner &&
            game->smallGrids[i][1].winner == game->smallGrids[i][2].winner &&
            game->smallGrids[i][0].winner != ' ') {
            return (game->smallGrids[i][0].winner == 'x') ? 10 : -10;
        }
        // Vérifier les colonnes
        if (game->smallGrids[0][i].winner == game->smallGrids[1][i].winner &&
            game->smallGrids[1][i].winner == game->smallGrids[2][i].winner &&
            game->smallGrids[0][i].winner != ' ') {
            return (game->smallGrids[0][i].winner == 'x') ? 10 : -10;
        }
    }
    // Vérifier les diagonales
    if (game->smallGrids[0][0].winner == game->smallGrids[1][1].winner &&
        game->smallGrids[1][1].winner == game->smallGrids[2][2].winner &&
        game->smallGrids[0][0].winner != ' ') {
        return (game->smallGrids[0][0].winner == 'x') ? 10 : -10;
    }
    if (game->smallGrids[0][2].winner == game->smallGrids[1][1].winner &&
        game->smallGrids[1][1].winner == game->smallGrids[2][0].winner &&
        game->smallGrids[0][2].winner != ' ') {
        return (game->smallGrids[0][2].winner == 'x') ? 10 : -10;
    }
     // Compter les marqueurs si aucun alignement n'est construit et aucune case n'est disponible
    int countX = 0, countO = 0;
    int emptySpaces = 0;
    for (int gridRow = 0; gridRow < 3; gridRow++) {
        for (int gridCol = 0; gridCol < 3; gridCol++) {
            if (game->smallGrids[gridRow][gridCol].winner == ' ') emptySpaces++;
            if (game->smallGrids[gridRow][gridCol].winner == 'o') countO++;
            if (game->smallGrids[gridRow][gridCol].winner == 'x') countX++;
        }
    }
    if (countX >= 5) return 10;  // 'x' gagne
    else if (countO >= 5) return -10; // 'o' gagne
    if (emptySpaces==0)
    {
        return 0;
    }
    else
    return countX-countO; // draw
}

int evaluateGameStatepro(SuperMorpion *game) {
    // Vérifier les lignes et les colonnes pour la victoire
    for (int i = 0; i < 3; i++) {
        // Vérifier les lignes
        if (game->smallGrids[i][0].winner == game->smallGrids[i][1].winner &&
            game->smallGrids[i][1].winner == game->smallGrids[i][2].winner &&
            game->smallGrids[i][0].winner != ' ') {
            return (game->smallGrids[i][0].winner == 'x') ? 10 : -10;
        }
        // Vérifier les colonnes
        if (game->smallGrids[0][i].winner == game->smallGrids[1][i].winner &&
            game->smallGrids[1][i].winner == game->smallGrids[2][i].winner &&
            game->smallGrids[0][i].winner != ' ') {
            return (game->smallGrids[0][i].winner == 'x') ? 10 : -10;
        }
    }
    // Vérifier les diagonales
    if (game->smallGrids[0][0].winner == game->smallGrids[1][1].winner &&
        game->smallGrids[1][1].winner == game->smallGrids[2][2].winner &&
        game->smallGrids[0][0].winner != ' ') {
        return (game->smallGrids[0][0].winner == 'x') ? 10 : -10;
    }
    if (game->smallGrids[0][2].winner == game->smallGrids[1][1].winner &&
        game->smallGrids[1][1].winner == game->smallGrids[2][0].winner &&
        game->smallGrids[0][2].winner != ' ') {
        return (game->smallGrids[0][2].winner == 'x') ? 10 : -10;
    }
     // Compter les marqueurs si aucun alignement n'est construit et aucune case n'est disponible
    int countX = 0, countO = 0;
    int emptySpaces = 0;
    for (int gridRow = 0; gridRow < 3; gridRow++) {
        for (int gridCol = 0; gridCol < 3; gridCol++) {
            if (game->smallGrids[gridRow][gridCol].winner == ' ') emptySpaces++;
            if (game->smallGrids[gridRow][gridCol].winner == 'o') countO++;
            if (game->smallGrids[gridRow][gridCol].winner == 'x') countX++;
        }
    }
    if (countX >= 5) return 10;  // 'x' gagne
    else if (countO >= 5) return -10; // 'o' gagne
    return 0; // draw
}

int isFinal(SuperMorpion *game) {
        // Vérifier les lignes et les colonnes pour la victoire
    for (int i = 0; i < 3; i++) {
        // Vérifier les lignes
        if (game->smallGrids[i][0].winner == game->smallGrids[i][1].winner &&
            game->smallGrids[i][1].winner == game->smallGrids[i][2].winner &&
            game->smallGrids[i][0].winner != ' ') {
            return 1;
        }
        // Vérifier les colonnes
        if (game->smallGrids[0][i].winner == game->smallGrids[1][i].winner &&
            game->smallGrids[1][i].winner == game->smallGrids[2][i].winner &&
            game->smallGrids[0][i].winner != ' ') {
            return 1;
        }
    }
    // Vérifier les diagonales
    if (game->smallGrids[0][0].winner == game->smallGrids[1][1].winner &&
        game->smallGrids[1][1].winner == game->smallGrids[2][2].winner &&
        game->smallGrids[0][0].winner != ' ') {
        return 1;
    }
    if (game->smallGrids[0][2].winner == game->smallGrids[1][1].winner &&
        game->smallGrids[1][1].winner == game->smallGrids[2][0].winner &&
        game->smallGrids[0][2].winner != ' ') {
        return 1;
    }
     // Compter les marqueurs si aucun alignement n'est construit et aucune case n'est disponible
    int countX = 0, countO = 0;
    int emptySpaces = 0;
    for (int gridRow = 0; gridRow < 3; gridRow++) {
        for (int gridCol = 0; gridCol < 3; gridCol++) {
            if (game->smallGrids[gridRow][gridCol].winner == ' ') emptySpaces++;
            if (game->smallGrids[gridRow][gridCol].winner == 'o') countO++;
            if (game->smallGrids[gridRow][gridCol].winner == 'x') countX++;

                    }
                }
    // Si toutes les cases sont remplies et aucun alignement n'a été construit
    if (emptySpaces == 0 || countO>=5 || countX>=5) {
        return 1; // Match nul si aucun joueur n'a 5 marqueurs
    }
    return 0; // Le jeu continue
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}
