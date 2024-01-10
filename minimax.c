#include <stdio.h>
#include <time.h>
#include <string.h>
#include "minimax.h"

int MAXdep=12;

void setHorizon(int horizon){
    MAXdep=horizon;
}

int superminimax(SuperMorpion *game, int depth, char player,int alpha, int beta) {
    clock_t now=clock();
    double timetaken=(double)(now-game->start_time)/ CLOCKS_PER_SEC;
    if (depth >= MAXdep || isFinal(game) || timetaken>game->timelimit) {
        int score = evaluateGameState(game);
        int playersign= (player=='x') ? 1 : -1;
        return playersign * score; // Retourner le score si jeu terminé ou profondeur max atteinte
    }
    int bestScore = -100 ;
    char stat=game->smallGrids[game->lastMoveRow][game->lastMoveCol].winner;
    if(stat=='x' || stat=='o' || stat=='d' ){
        game->lastMoveRow=-1;
        game->lastMoveCol=-1;
    }
    int lastMoveRow=game->lastMoveRow;
    int lastMoveCol=game->lastMoveCol;
    // Si c'est le premier coup du jeu ou la grille cible est pleine/gagnée, considérer toutes les grilles
    for (int gridRow = 0; gridRow < 3; gridRow++) {
        for (int gridCol = 0; gridCol < 3; gridCol++) 
            if(game->smallGrids[gridRow][gridCol].winner==' '){
                GameState *grid = &game->smallGrids[gridRow][gridCol];
                for (int row = 0; row < 3; row++) {
                    for (int col = 0; col < 3; col++) {
                        if (grid->grid[row][col] == ' ') {
                            
                            // Jouer le coup
                            if(playMove(game, (gridRow) * 3 + (gridCol) , row, col)){
                                // Appel récursif de minimax 
                                int currentScore = - superminimax(game, depth + 1, (player == 'x') ? 'o' : 'x',-beta, -alpha);
                                // Annuler le coup
                                grid->grid[row][col] = ' ';
                                grid->winner=' ';
                                game->currentPlayer = player; // Restaurer le joueur actuel
                                game->lastMoveRow=lastMoveRow;
                                game->lastMoveCol=lastMoveCol;
                                // Mettre à jour le meilleur score            
                                // la coupure alpha-bêta
                                alpha = max(alpha, currentScore);
                                if (alpha >= beta || alpha==10) {
                                    return alpha; // Coupure bêta
                                }
                            }  
                        }
                             
                    }
                }
        }
    }
    return alpha;
}

void computerMove(SuperMorpion *game) {
    int bestScore = -1000;
    int alpha = -1000;
    int beta = 1000;
    int bestMoveRow, bestMoveCol, bestGridRow, bestGridCol;
    char opponentPlayer = (game->currentPlayer == 'x') ? 'o' : 'x';
    char currentPlayer = game->currentPlayer;
    int lastMove;
    char stat=game->smallGrids[game->lastMoveRow][game->lastMoveCol].winner;
    if(stat=='x' || stat=='o' || stat=='d' ){
        game->lastMoveRow=-1;
        game->lastMoveCol=-1;

    }
    int lastMoveRow=game->lastMoveRow;
    int lastMoveCol=game->lastMoveCol;
    // Parcourir toutes les petites grilles et toutes les cases pour trouver le meilleur coup
    for (int gridRow = 0; gridRow < 3; gridRow++) {
        for (int gridCol = 0; gridCol < 3; gridCol++) 
        if(game->smallGrids[gridRow][gridCol].winner==' '){
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) 
                    if(game->smallGrids[gridRow][gridCol].grid[row][col]==' '){
                    // Vérifier si le coup est valide
                    if (playMove(game, gridRow*3+gridCol,row,col)) {
                        // Jouer le coup
                        
                        //game->smallGrids[gridRow][gridCol].grid[row][col] = currentPlayer;
                       // updateGridState(&game->smallGrids[gridRow][gridCol]);
                        //game->currentPlayer=opponentPlayer;
                        // Appeler minimax
                        int score = -superminimax(game, 0, opponentPlayer, -beta, -alpha); 
                        // Annuler le coup
                        game->smallGrids[gridRow][gridCol].grid[row][col] = ' ';
                        updateGridState(&game->smallGrids[gridRow][gridCol]);
                        game->currentPlayer=currentPlayer;
                        game->lastMoveRow=lastMoveRow;
                        game->lastMoveCol=lastMoveCol;
                        if(gridRow==1)
                        {
                        int ikk=1;
                        }
                        // Mettre à jour le meilleur score et le meilleur coup
                        if (score > bestScore) {
                            bestScore = score;
                            bestGridRow = gridRow;
                            bestGridCol = gridCol;
                            bestMoveRow = row;
                            bestMoveCol = col;
                        }
                    }
                }
            }
        }
    }
    printf("%d%d\n",bestGridRow*3+bestGridCol+1,bestMoveRow*3+bestMoveCol+1);
    if(evaluateGameState(game)!=0) return;
    // Jouer le meilleur coup trouvé
    //game->smallGrids[bestGridRow][bestGridCol].grid[bestMoveRow][bestMoveCol] = currentPlayer;
    //game->lastMoveCol=bestMoveCol;
    //game->lastMoveRow=bestMoveRow;
    //game->currentPlayer = (currentPlayer == 'x') ? 'o' : 'x';
    playMove(game,bestGridRow*3+bestGridCol,bestMoveRow,bestMoveCol);
}