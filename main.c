#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include "morpion.h"
//#include "supermorpion.h"
#include "minimax.h"

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

   /*
    printf("digraph {\n");
    generateDecisionTree(&gameState,-1);
    printf("}\n");
    */

    SuperMorpion game;
    initializeSuperMorpion(&game);
    //game.smallGrids[1][1].grid[1][1]='x';
    game.currentPlayer='x';
    int gameOver=0;
    while (!gameOver) {


        if (!inputMove(&game)) {
            // Demande à nouveau si la saisie est invalide
            continue;
        }

        displayGame(&game);

        computerMove(&game);

        // ... affichage du jeu ...
        displayGame(&game);

         FILE *file = fopen("output.dot", "w");
        displaySuperMorpionGraphviz(&game,file); // Appel de la fonction qui écrit dans stdout
        fclose(file);

        system("dot output.dot -T png -o output.png");

        // Vérifier l'état du jeu
        gameOver= evaluateGameState(&game);
        if (gameOver != 0) break; // Le jeu est terminé


    }
    if (gameOver == 10) {
        printf("Joueur 'x' gagne!\n");
    } else if (gameOver == -10) {
        printf("Joueur 'o' gagne!\n");
    } else {
        printf("Match nul!\n");
    }
}