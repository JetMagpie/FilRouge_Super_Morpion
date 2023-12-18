#ifndef SUPER_MORPION_H
    #define SUPER_MORPION_H

typedef struct supermorpion
{
    int stat[10][10]; //-1(not occupier) or 0(white) or 1(black)
    int morpion_fini[10];// -1(available) or 0(white win) or 1(black win) 
    int lmp; // last move position 1~81
    int p;//Now turn 0(white) or 1(black)
}morpion;

typedef struct supermorpion_noeud//noeud of the tree
{
    morpion M;
    noeud * nextM[];//pointer to all child noeuds
}noeud;


#endif