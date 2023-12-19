#include "supermorpion.h"

void showposition(morpion M){
    char **S;
    S=(char **)malloc(18*sizeof(char));
    for(int i=0;i<9;i++) S=(char *)malloc(18*sizeof(char));
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            switch (M.morpion_fini[i][j])
            {
            case PLAYERO:
                sprintf(&S[6*i+0][6*j],"+-+-+-");
                sprintf(&S[6*i+1][6*j],"|     ");
                sprintf(&S[6*i+2][6*j],"+-+-+-");
                sprintf(&S[6*i+3][6*j],"|  O  ");
                sprintf(&S[6*i+4][6*j],"+-+-+-");
                sprintf(&S[6*i+5][6*j],"|     ");
                break;
            case PLAYERX:
                sprintf(&S[6*i+0][6*j],"+-+-+-");
                sprintf(&S[6*i+1][6*j],"|     ");
                sprintf(&S[6*i+2][6*j],"+-+-+-");
                sprintf(&S[6*i+3][6*j],"|  X  ");
                sprintf(&S[6*i+4][6*j],"+-+-+-");
                sprintf(&S[6*i+5][6*j],"|     ");
                break;
            case EMPTY:
                sprintf(&S[6*i+0][6*j],"+-+-+-");
                sprintf(&S[6*i+1][6*j],"|%c|%c|%c",PlayertoChar(M.stat[3*i+0][3*j]),PlayertoChar(M.stat[3*i+0][3*j+1]),PlayertoChar(M.stat[3*i+0][3*j+2]));
                sprintf(&S[6*i+2][6*j],"+-+-+-");
                sprintf(&S[6*i+3][6*j],"|%c|%c|%c",PlayertoChar(M.stat[3*i+1][3*j]),PlayertoChar(M.stat[3*i+1][3*j+1]),PlayertoChar(M.stat[3*i+1][3*j+2]));
                sprintf(&S[6*i+4][6*j],"+-+-+-");
                sprintf(&S[6*i+5][6*j],"|%c|%c|%c",PlayertoChar(M.stat[3*i+2][3*j]),PlayertoChar(M.stat[3*i+2][3*j+1]),PlayertoChar(M.stat[3*i+2][3*j+2]));
            }
        }
    }
    for(int i=0;i<19;i++){
        printf("%s|\n",S[i]);
    }
    printf("+-+-+-+-+-+-+-+-+-+\n");
}

char PlayertoChar(int player){
    switch (player)
    {
    case PLAYERO:
        return 'O';
        break;
    case PLAYERX:
        return 'X';
        break;
    case EMPTY:
        return ' ';
        break;
    default:
        break;
    }
}

int tomorpion(int x, int y, int player,morpion * M){
    if(x<1||y<1||x>9||y>9||player!=PLAYERO||player!=PLAYERX){
        printf("Out of range!\n");
        return 0;
    }
    M->lmp[0]=x;
    M->lmp[1]=y;
    M->p=player;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(M->stat[3*i][3*j]+M->stat[3*i+1][3*j+1]+M->stat[3*i+2][3*j+2]==3*PLAYERO
            ||M->stat[3*i+2][3*j]+M->stat[3*i+1][3*j+1]+M->stat[3*i][3*j+2]==3*PLAYERO
            ||M->stat[3*i][3*j]+M->stat[3*i][3*j+1]+M->stat[3*i][3*j+2]==3*PLAYERO
            ||M->stat[3*i+1][3*j]+M->stat[3*i+1][3*j+1]+M->stat[3*i+1][3*j+2]==3*PLAYERO
            ||M->stat[3*i+2][3*j]+M->stat[3*i+2][3*j+1]+M->stat[3*i+2][3*j+2]==3*PLAYERO
            ||M->stat[3*i][3*j]+M->stat[3*i+1][3*j]+M->stat[3*i+2][3*j]==3*PLAYERO
            ||M->stat[3*i][3*j+1]+M->stat[3*i+1][3*j+1]+M->stat[3*i+2][3*j+1]==3*PLAYERO
            ||M->stat[3*i][3*j+2]+M->stat[3*i+1][3*j+2]+M->stat[3*i+2][3*j+2]==3*PLAYERO)
            M->morpion_fini[i][j]=PLAYERO;
            else if(M->stat[3*i][3*j]+M->stat[3*i+1][3*j+1]+M->stat[3*i+2][3*j+2]==3*PLAYERX
            ||M->stat[3*i+2][3*j]+M->stat[3*i+1][3*j+1]+M->stat[3*i][3*j+2]==3*PLAYERX
            ||M->stat[3*i][3*j]+M->stat[3*i][3*j+1]+M->stat[3*i][3*j+2]==3*PLAYERX
            ||M->stat[3*i+1][3*j]+M->stat[3*i+1][3*j+1]+M->stat[3*i+1][3*j+2]==3*PLAYERX
            ||M->stat[3*i+2][3*j]+M->stat[3*i+2][3*j+1]+M->stat[3*i+2][3*j+2]==3*PLAYERX
            ||M->stat[3*i][3*j]+M->stat[3*i+1][3*j]+M->stat[3*i+2][3*j]==3*PLAYERX
            ||M->stat[3*i][3*j+1]+M->stat[3*i+1][3*j+1]+M->stat[3*i+2][3*j+1]==3*PLAYERX
            ||M->stat[3*i][3*j+2]+M->stat[3*i+1][3*j+2]+M->stat[3*i+2][3*j+2]==3*PLAYERX)
            M->morpion_fini[i][j]=PLAYERX;
        }
    }
    return 1;
}

morpion *createmorpion(){
    morpion *M=(morpion *)malloc(sizeof(morpion));
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            M->morpion_fini[i][j]=EMPTY;
        }
    }
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            M->stat[i][j]=EMPTY;
        }
    }
    M->p=PLAYERX;
    return M;
}