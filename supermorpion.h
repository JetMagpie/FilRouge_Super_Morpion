#include <stdio.h>
#include <string.h>
#include "morpion.h"

typedef struct {
    GameState smallGrids[3][3]; // 9 petites grilles pour le super-morpion
    char currentPlayer;
} SuperMorpion;

void displayGame(SuperMorpion *game);

void initializeSuperMorpion(SuperMorpion *game) ;

int inputMove(SuperMorpion *game);

char* generateSmallGridGraphviz(GameState grid) ;

void displaySuperMorpionGraphviz(SuperMorpion *game, FILE *file) ;