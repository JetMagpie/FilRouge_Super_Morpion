#include <stdio.h>
#include <string.h>
#include "minimax.h"

int MAXdep=7;

int min(int a, int b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
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







void playMove(SuperMorpion *game, int gridIndex, int rowIndex, int colIndex) {
    // Valider le coup
    if (!validateMove(game, gridIndex, rowIndex, colIndex)) {
        return; // Coup invalide, ne pas continuer
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
}

int superminimax(SuperMorpion *game, int depth, char player) {
    int score = evaluateGameState(game);
    if (depth >= MAXdep || score != 0) {
        return score; // Retourner le score si jeu terminé ou profondeur max atteinte
    }

    int bestScore = (player == 'x') ? -1000 : 1000;
    int targetGridRow = game->lastMoveRow;
    int targetGridCol = game->lastMoveCol;

    // Si c'est le premier coup du jeu ou la grille cible est pleine/gagnée, considérer toutes les grilles
    if (game->lastMoveRow == -1 || game->smallGrids[targetGridRow][targetGridCol].winner != ' ') {
        targetGridRow = -1;
        targetGridCol = -1;
    }

    for (int gridRow = 0; gridRow < 3; gridRow++) {
        for (int gridCol = 0; gridCol < 3; gridCol++) {
            if (targetGridRow == -1 || (gridRow == targetGridRow && gridCol == targetGridCol)) {
                GameState *grid = &game->smallGrids[gridRow][gridCol];
                for (int row = 0; row < 3; row++) {
                    for (int col = 0; col < 3; col++) {
                        if (grid->grid[row][col] == ' ') {
                            // Jouer le coup
                            playMove(game, gridRow * 3 + gridCol, row, col);
                            
                            // Appel récursif de minimax
                            int currentScore = superminimax(game, depth + 1, (player == 'x') ? 'o' : 'x');

                            // Annuler le coup
                            grid->grid[row][col] = ' ';
                            grid->winner = ' ';
                            game->currentPlayer = player; // Restaurer le joueur actuel

                            // Mettre à jour le meilleur score
                            bestScore = (player == 'x') ? max(bestScore, currentScore) : min(bestScore, currentScore);
                        }
                    }
                }
            }
        }
    }
    return bestScore;
}

int evaluateGameState(SuperMorpion *game) {
    // Vérifier si l'un des joueurs a gagné dans la grande grille
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            char winner = game->smallGrids[row][col].winner;
            if (winner == 'x' || winner == 'o') {
                // Évaluer les lignes, colonnes et diagonales pour la victoire
                if ((game->smallGrids[row][0].winner == winner && 
                     game->smallGrids[row][1].winner == winner && 
                     game->smallGrids[row][2].winner == winner) ||
                    (game->smallGrids[0][col].winner == winner && 
                     game->smallGrids[1][col].winner == winner && 
                     game->smallGrids[2][col].winner == winner) ||
                    (game->smallGrids[0][0].winner == winner && 
                     game->smallGrids[1][1].winner == winner && 
                     game->smallGrids[2][2].winner == winner) ||
                    (game->smallGrids[0][2].winner == winner && 
                     game->smallGrids[1][1].winner == winner && 
                     game->smallGrids[2][0].winner == winner)) {
                    return (winner == 'x') ? 10 : -10;
                }
            }
        }
    }

     // Compter les marqueurs si aucun alignement n'est construit et aucune case n'est disponible
    int countX = 0, countO = 0;
    int emptySpaces = 0;

    for (int gridRow = 0; gridRow < 3; gridRow++) {
        for (int gridCol = 0; gridCol < 3; gridCol++) {
            GameState *grid = &game->smallGrids[gridRow][gridCol];
            if (grid->winner == ' ') { // Si la grille n'a pas encore de gagnant
                for (int row = 0; row < 3; row++) {
                    for (int col = 0; col < 3; col++) {
                        if (grid->grid[row][col] == 'x') countX++;
                        else if (grid->grid[row][col] == 'o') countO++;
                        else emptySpaces++;
                    }
                }
            }
        }
    }

    // Si toutes les cases sont remplies et aucun alignement n'a été construit
    if (emptySpaces == 0) {
        if (countX >= 5) return 10;  // 'x' gagne
        else if (countO >= 5) return -10; // 'o' gagne
        else return 0; // Match nul si aucun joueur n'a 5 marqueurs
    }

    return 0; // Le jeu continue
}

void computerMove(SuperMorpion *game) {
    int bestScore = -1000;
    int bestMoveRow, bestMoveCol, bestGridRow, bestGridCol;
    char opponentPlayer = (game->currentPlayer == 'x') ? 'o' : 'x';
    char currentPlayer = game->currentPlayer;
    char stat=game->smallGrids[game->lastMoveRow][game->lastMoveCol].winner;
    if(stat=='x' || stat=='o' || stat=='d' ){
        game->lastMoveRow=-1;
        game->lastMoveCol=-1;

    }
    int lastMoveRow=game->lastMoveRow;
    int lastMoveCol=game->lastMoveCol;

    // Parcourir toutes les petites grilles et toutes les cases pour trouver le meilleur coup
    for (int gridRow = 0; gridRow < 3; gridRow++) {
        for (int gridCol = 0; gridCol < 3; gridCol++) {
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    // Vérifier si le coup est valide
                    if (validateMove(game, gridRow*3+gridCol , row, col)) {
                        // Jouer le coup
                        //playMove(game, gridRow*3+gridCol,row,col);
                        game->smallGrids[gridRow][gridCol].grid[row][col] = currentPlayer;
                        // Appeler minimax
                        int score = superminimax(game, 0, opponentPlayer); 
                        // Annuler le coup
                        game->smallGrids[gridRow][gridCol].grid[row][col] = ' ';
                        game->lastMoveRow=lastMoveRow;
                        game->lastMoveCol=lastMoveCol;

                        // Mettre à jour le meilleur score et le meilleur coup
                        if (score > bestScore) {
                            bestScore = score;
                            bestGridRow = gridRow;
                            bestGridCol = gridCol;
                            bestMoveRow = row;
                            bestMoveCol = col;
                        }
                    }
                }
            }
        }
    }

    // Jouer le meilleur coup trouvé
    game->smallGrids[bestGridRow][bestGridCol].grid[bestMoveRow][bestMoveCol] = currentPlayer;
    game->lastMoveCol=bestMoveCol;
    game->lastMoveRow=bestMoveRow;
    game->currentPlayer = (currentPlayer == 'x') ? 'o' : 'x';
}
