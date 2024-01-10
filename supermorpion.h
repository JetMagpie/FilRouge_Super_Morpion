#include <stdio.h>
#include <string.h>
#include <time.h>
#include "morpion.h"

#ifndef SUPERMORPION_H
#define SUPERMORPION_H

typedef struct {
    GameState smallGrids[3][3]; // 9 petites grilles pour le super-morpion
    char currentPlayer;
    int lastMoveRow; // Dernière ligne jouée
    int lastMoveCol; // Dernière colonne jouée
    int timelimit;
    clock_t start_time;
} SuperMorpion;

void displayGame(SuperMorpion *game);

void initializeSuperMorpion(SuperMorpion *game) ;

int inputMove(SuperMorpion *game);

char* generateSmallGridGraphviz(GameState grid) ;

void displaySuperMorpionGraphviz(SuperMorpion *game, FILE *file) ;

int validateMove(SuperMorpion *game, int gridIndex, int rowIndex, int colIndex) ;

void updateGridState(GameState *grid) ;

void superparseFEN(SuperMorpion *game, const char *fen) ;

int isGridPlayable(GameState *grid);

int playMove(SuperMorpion *game, int gridIndex, int rowIndex, int colIndex);

int evaluateGameState(SuperMorpion *game) ;

int isFinal(SuperMorpion *game) ;

int min(int a, int b) ;

int max(int a, int b) ;

#endif
