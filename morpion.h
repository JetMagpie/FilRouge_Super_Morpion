#include <stdio.h>
#include <string.h>

// Structure pour représenter l'état du jeu
typedef struct {
    char grid[3][3];
    char currentPlayer; // 'x' ou 'o'
    char winner; // 'x', 'o', ou ' ' si pas encore de gagnant
    int minimax;
} GameState;

// Fonction pour initialiser la grille de jeu
void initializeGrid(GameState *gameState) ;

// Fonction pour analyser la chaîne FEN
void parseFEN(char *fen, GameState *gameState) ;

// Fonction pour vérifier l'état du jeu (victoire, défaite, match nul ou continuer)
int checkGameOver(GameState *gameState, char player) ;

// Fonction minimax
int minimax(GameState *gameState, char player) ;

void generateGraphvizNode(GameState *gameState, int nodeId) ;

void generateDecisionTree(GameState *gameState, int parentId) ;