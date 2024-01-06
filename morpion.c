#include <stdio.h>
#include <string.h>
#include "morpion.h"



// Fonction pour initialiser la grille de jeu
void initializeGrid(GameState *gameState) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            gameState->grid[i][j] = ' '; // Initialiser toutes les cellules à vide
        }
    }
}

// Fonction pour analyser la chaîne FEN
void parseFEN(char *fen, GameState *gameState) {
    initializeGrid(gameState); // Initialiser la grille

    int row = 0, col = 0;

    for (int i = 0; fen[i] != ' ' && fen[i] != '\0'; ++i) {
        if (fen[i] == 'x' || fen[i] == 'o') {
            gameState->grid[row][col] = fen[i];
            col++;
        } else if (fen[i] >= '1' && fen[i] <= '9') {
            int emptySpaces = fen[i] - '0';
            col += emptySpaces;
        }

        if (col >= 3) {
            row++;
            col = 0;
        }
    }

    // Déterminer le joueur actuel
    if (fen[strlen(fen) - 1] == 'o' || fen[strlen(fen) - 1] == 'x') {
        gameState->currentPlayer = fen[strlen(fen) - 1];
    }
}

// Fonction pour vérifier l'état du jeu (victoire, défaite, match nul ou continuer)
int checkGameOver(GameState *gameState, char player) {
    char opponent = (player == 'x') ? 'o' : 'x';
    int playerScore = (player == 'x') ? 1 : -1;
    int openSpots = 0;

    // Vérifier les victoires pour les joueurs
    for (int i = 0; i < 3; i++) {
        // Vérifier les lignes et les colonnes
        if ((gameState->grid[i][0] == player && gameState->grid[i][1] == player && gameState->grid[i][2] == player) ||
            (gameState->grid[0][i] == player && gameState->grid[1][i] == player && gameState->grid[2][i] == player)) {
            return playerScore;
        }

        // Vérifier les diagonales
        if ((gameState->grid[0][0] == player && gameState->grid[1][1] == player && gameState->grid[2][2] == player) ||
            (gameState->grid[0][2] == player && gameState->grid[1][1] == player && gameState->grid[2][0] == player)) {
            return playerScore;
        }

        // Compter les cases vides
        for (int j = 0; j < 3; j++) {
            if (gameState->grid[i][j] == ' ') {
                openSpots++;
            }
        }
    }

    // Si l'opposant a gagné
    for (int i = 0; i < 3; i++) {
        if ((gameState->grid[i][0] == opponent && gameState->grid[i][1] == opponent && gameState->grid[i][2] == opponent) ||
            (gameState->grid[0][i] == opponent && gameState->grid[1][i] == opponent && gameState->grid[2][i] == opponent) ||
            (gameState->grid[0][0] == opponent && gameState->grid[1][1] == opponent && gameState->grid[2][2] == opponent) ||
            (gameState->grid[0][2] == opponent && gameState->grid[1][1] == opponent && gameState->grid[2][0] == opponent)) {
            return -playerScore;
        }
    }

    // Vérifier s'il y a un match nul
    if (openSpots == 0) {
        return 0; // Match nul
    }

    return 2; // Continuer le jeu
}


// Fonction minimax
int minimax(GameState *gameState, char player) {
    int score = checkGameOver(gameState, player);
    if (score != 2) return score;

    int bestScore = (player == 'x') ? -10 : 10; // Initialisation pour maximiser ou minimiser

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Vérifiez si la case (i, j) est vide
            if (gameState->grid[i][j] == ' ') {
                // Jouez le coup
                gameState->grid[i][j] = player;

                // Appel récursif de minimax pour le prochain joueur
                int currentScore = minimax(gameState, (player == 'x') ? 'o' : 'x');

                // Annulez le coup joué
                gameState->grid[i][j] = ' ';

                // Mettez à jour le meilleur score
                if (player == 'x') {
                    bestScore = (currentScore > bestScore) ? currentScore : bestScore;
                } else {
                    bestScore = (currentScore < bestScore) ? currentScore : bestScore;
                }
            }
        }
    }

    return bestScore;
}

