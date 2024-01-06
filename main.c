#include <stdio.h>
#include <string.h>
#include "morpion.h"
//#include "supermorpion.h"

int main(){

    //test 
    GameState gameState;
    parseFEN("o2xoox2 x", &gameState);

 /*   // Afficher la grille et le joueur actuel pour tester
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%c ", gameState.grid[i][j]);
        }
        printf("\n");
    }
    printf("Joueur actuel : %c\n", gameState.currentPlayer);

    printf("Winner : %d\n",minimax(&gameState,gameState.currentPlayer));
    */

   printf("digraph {\n");
    generateDecisionTree(&gameState,-1);
    printf("}\n");
}