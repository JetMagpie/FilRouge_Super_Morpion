#include <stdio.h>
#include <string.h>
#include "supermorpion.h"



int min(int a, int b) ;

int max(int a, int b) ;

int isGridPlayable(GameState *grid);





void playMove(SuperMorpion *game, int gridIndex, int rowIndex, int colIndex) ;

int superminimax(SuperMorpion *game, int depth, char player) ;

int evaluateGameState(SuperMorpion *game) ;

void computerMove(SuperMorpion *game);