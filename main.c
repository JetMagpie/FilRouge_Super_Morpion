#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include "morpion.h"
//#include "supermorpion.h"
#include "minimax.h"

int main(int argc,char *argv[]){

#ifdef TTTREE_ENABLE
    GameState gameState;
    parseFEN(argv[1], &gameState);
    printf("digraph {\n");
    generateDecisionTree(&gameState,-1);
    printf("}\n");
#endif
   
//#ifdef SM_REFRESH_ENABLE   
    SuperMorpion game;
    initializeSuperMorpion(&game);
    //game.smallGrids[1][1].grid[1][1]='x';
  char *str="ox4xoxOOOX2xooox1oXx2xo4oox4ox 84 x";
  superparseFEN(&game,str);
    displayGame(&game);
    for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
    {
        updateGridState(&game.smallGrids[i][j]);
    }

    
    //game.currentPlayer='x';
    int gameOver=0;
    while (!isFinal(&game)) {
        int p=1;

        if (!inputMove(&game)) {
           // Demande à nouveau si la saisie est invalide
           continue;
        }

        //displayGame(&game);
        printf("\n");
        computerMove(&game);

        // ... affichage du jeu ...
        displayGame(&game);

         FILE *file = fopen("output.dot", "w");
        displaySuperMorpionGraphviz(&game,file); // Appel de la fonction qui écrit dans stdout
        fclose(file);

        system("dot output.dot -T png -o output.png");

        // Vérifier l'état du jeu
        gameOver= evaluateGameState(&game);



    }
    if (gameOver == 10) {
        printf("Joueur 'x' gagne!\n");
    } else if (gameOver == -10) {
        printf("Joueur 'o' gagne!\n");
    } else {
        printf("Match nul!\n");
    }
//#endif
    
}