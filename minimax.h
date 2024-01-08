#include <stdio.h>
#include <string.h>
#include "supermorpion.h"




int min(int a, int b) ;

int max(int a, int b) ;

int isGridPlayable(GameState *grid);





int playMove(SuperMorpion *game, int gridIndex, int rowIndex, int colIndex) ;

int superminimax(SuperMorpion *game, int depth, char player,int fathervalue) ;

int evaluateGameState(SuperMorpion *game) ;

void computerMove(SuperMorpion *game);

int isFinal(SuperMorpion *game) ;