#include <stdio.h>
#include <string.h>
#include "supermorpion.h"

#ifndef MINIMAX_H
#define MINIMAX_H

void setHorizon(int horizon);

int superminimax(SuperMorpion *game, int depth, char player,int alpha, int beta) ;

void computerMove(SuperMorpion *game);

#endif
