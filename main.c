#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "morpion.h"
#include "supermorpion.h"
#include "minimax.h"

int main(int argc,char *argv[]){
    

#ifdef TTTREE_ENABLE
    GameState gameState;
    parseFEN(argv[1], &gameState);
    printf("digraph {\n");
    generateDecisionTree(&gameState,-1);
    printf("}\n");
#endif

#ifdef SM_REFRESH_ENABLE   
    SuperMorpion game;
    initializeSuperMorpion(&game);
    //printf("Entrez nombre d'horizon:");
    //int horizon;
    //scanf("%d",&horizon);
    //setHorizon(horizon);
    if(argc>1)setHorizon(argv[1]-'0');
    displayGame(&game);
    for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
    {
        updateGridState(&game.smallGrids[i][j]);
    }
    //game.currentPlayer='x';
    int gameOver=0;
    while (!isFinal(&game)) {
        if (!inputMove(&game)) {
           // Demande à nouveau si la saisie est invalide
           continue;
        }
        //displayGame(&game);
        printf("\n");
        computerMove(&game);
        // ... affichage du jeu ...
        displayGame(&game);
         FILE *file = fopen("g.dot", "w");
        displaySuperMorpionGraphviz(&game,file); // Appel de la fonction qui écrit dans stdout
        fclose(file);
        char *smPath = getenv("SMPATH");
        if (smPath != NULL){
            char* outputCmd;
            sprintf(outputCmd,"dot g.dot -T png -o %s/g.png",smPath);
            system(outputCmd);
        }
        else system("dot g.dot -T png -o g.png");
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
#endif

//#ifdef SM_BOT_ENABLE
    clock_t start_time=clock();
    if(argc<2)return 0;
    SuperMorpion game;
    initializeSuperMorpion(&game);
    int gameOver=0;
    //char *str="ox4xoxOOOX2xooox1oXx2xo4oox4ox 84 x";
    superparseFEN(&game, argv[1]);
    game.timelimit=atoi(argv[2]);
    game.start_time=start_time;
    if (!isFinal(&game)) {
        computerMove(&game);
    }

    gameOver= evaluateGameState(&game);
    if(isFinal(&game)){
        if (gameOver == 10) {
            printf("Joueur 'x' gagne!\n");
        } else if (gameOver == -10) {
            printf("Joueur 'o' gagne!\n");
        } else {
            printf("Match nul!\n");
        }
    }
//#endif
    
}