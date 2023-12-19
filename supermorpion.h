#include <stdio.h>
#include <stdlib.h>

#ifndef SUPER_MORPION_H
    #define SUPER_MORPION_H

#define PLAYERX 88
#define PLAYERO 79
#define EMPTY -1

typedef struct supermorpion
{
    int stat[9][9]; //the chessboard
    int morpion_fini[3][3];// stat of each chessboard
    int lmp[2]; // last move position
    int p;//last player
}morpion;

typedef struct supermorpion_noeud//noeud of the tree
{
    morpion M;
    noeud * nextM[];//pointer to all child noeuds
}noeud;

void showposition(morpion M);//print the game to the terminal
int tomorpion(int x, int y, int player,morpion* M);//save a step
morpion * createmorpion();//creat a new game


#endif