#include <stdio.h>
#include <string.h>
#include "minimax.h"

int MAXdep=100;

int min(int a, int b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}


int isGridPlayable(GameState *grid) {
    if (grid->winner != ' ') return 0; // La grille a un gagnant

    // Vérifier si la grille est complète
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid->grid[i][j] == ' ') return 1; // Il y a de l'espace pour jouer
        }
    }
    return 0; // La grille est complète
}







int playMove(SuperMorpion *game, int gridIndex, int rowIndex, int colIndex) {
    // Valider le coup
    if (!validateMove(game, gridIndex, rowIndex, colIndex)) {
        return 0; // Coup invalide, ne pas continuer
    }

    // Coup valide, jouer le coup
    GameState *targetGrid = &game->smallGrids[gridIndex / 3][gridIndex % 3];
    targetGrid->grid[rowIndex][colIndex] = game->currentPlayer;

    // Mettre à jour l'état de la petite grille
    updateGridState(targetGrid);

    // Changer de joueur
    game->currentPlayer = (game->currentPlayer == 'x') ? 'o' : 'x';

    // Mettre à jour lastMoveRow et lastMoveCol pour la prochaine grille cible
    game->lastMoveRow = rowIndex;
    game->lastMoveCol = colIndex;
    return 1;
}

int superminimax(SuperMorpion *game, int depth, char player,int alpha, int beta) {
    int score = evaluateGameState(game);
    int playersign= (player=='x') ? 1 : -1;
    if (depth >= MAXdep || score != 0) {
        return playersign * score; // Retourner le score si jeu terminé ou profondeur max atteinte
    }
    else{
        if(isFinal(game)) return 0;
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
        if(game->smallGrids[gridRow][gridCol].winner==' ')
            {
                GameState *grid = &game->smallGrids[gridRow][gridCol];
                for (int row = 0; row < 3; row++) {
                    for (int col = 0; col < 3; col++) {
                        if (grid->grid[row][col] == ' ') {
                            
                            // Jouer le coup
                            if(playMove(game, (gridRow) * 3 + (gridCol) , row, col))
                            {
            
                            // Appel récursif de minimax
                                
                            int currentScore = - superminimax(game, depth + 1, (player == 'x') ? 'o' : 'x',-beta, -alpha);

                            // Annuler le coup
                            grid->grid[row][col] = ' ';
                            updateGridState(grid);
                            game->currentPlayer = player; // Restaurer le joueur actuel

                            game->lastMoveRow=lastMoveRow;
                            game->lastMoveCol=lastMoveCol;
                            // Mettre à jour le meilleur score
                            
                            
                            // la coupure alpha-bêta
                            alpha = max(alpha, currentScore);
                        if (alpha >= beta) {
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

int evaluateGameState(SuperMorpion *game) {

    // Vérifier les lignes et les colonnes pour la victoire
    for (int i = 0; i < 3; i++) {
        // Vérifier les lignes
        if (game->smallGrids[i][0].winner == game->smallGrids[i][1].winner &&
            game->smallGrids[i][1].winner == game->smallGrids[i][2].winner &&
            game->smallGrids[i][0].winner != ' ') {
            return (game->smallGrids[i][0].winner == 'x') ? 10 : -10;
        }

        // Vérifier les colonnes
        if (game->smallGrids[0][i].winner == game->smallGrids[1][i].winner &&
            game->smallGrids[1][i].winner == game->smallGrids[2][i].winner &&
            game->smallGrids[0][i].winner != ' ') {
            return (game->smallGrids[0][i].winner == 'x') ? 10 : -10;
        }
    }

    // Vérifier les diagonales
    if (game->smallGrids[0][0].winner == game->smallGrids[1][1].winner &&
        game->smallGrids[1][1].winner == game->smallGrids[2][2].winner &&
        game->smallGrids[0][0].winner != ' ') {
        return (game->smallGrids[0][0].winner == 'x') ? 10 : -10;
    }
    if (game->smallGrids[0][2].winner == game->smallGrids[1][1].winner &&
        game->smallGrids[1][1].winner == game->smallGrids[2][0].winner &&
        game->smallGrids[0][2].winner != ' ') {
        return (game->smallGrids[0][2].winner == 'x') ? 10 : -10;
    }

     // Compter les marqueurs si aucun alignement n'est construit et aucune case n'est disponible
    int countX = 0, countO = 0;
    int emptySpaces = 0;

    for (int gridRow = 0; gridRow < 3; gridRow++) {
        for (int gridCol = 0; gridCol < 3; gridCol++) {
            if (game->smallGrids[gridRow][gridCol].winner == ' ') emptySpaces++;
            if (game->smallGrids[gridRow][gridCol].winner == 'o') countO++;
            if (game->smallGrids[gridRow][gridCol].winner == 'x') countX++;

                    }
                }
            
        

        if (countX >= 5) return 10;  // 'x' gagne
        else if (countO >= 5) return -10; // 'o' gagne

    return 0; // Le jeu continue
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
                    if (validateMove(game, gridRow*3+gridCol , row, col)) {
                        // Jouer le coup
                        playMove(game, gridRow*3+gridCol,row,col);
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
    if(evaluateGameState(game)!=0) return;
    // Jouer le meilleur coup trouvé
    //game->smallGrids[bestGridRow][bestGridCol].grid[bestMoveRow][bestMoveCol] = currentPlayer;
    //game->lastMoveCol=bestMoveCol;
    //game->lastMoveRow=bestMoveRow;
    //game->currentPlayer = (currentPlayer == 'x') ? 'o' : 'x';
    playMove(game,bestGridRow*3+bestGridCol,bestMoveRow,bestMoveCol);
}

int isFinal(SuperMorpion *game) {
    

     // Compter les marqueurs si aucun alignement n'est construit et aucune case n'est disponible
    int countX = 0, countO = 0;
    int emptySpaces = 0;

    for (int gridRow = 0; gridRow < 3; gridRow++) {
        for (int gridCol = 0; gridCol < 3; gridCol++) {
            if (game->smallGrids[gridRow][gridCol].winner == ' ') emptySpaces++;
            if (game->smallGrids[gridRow][gridCol].winner == 'o') countO++;
            if (game->smallGrids[gridRow][gridCol].winner == 'x') countX++;

                    }
                }
            

    // Si toutes les cases sont remplies et aucun alignement n'a été construit
    if (emptySpaces == 0 || countO>=5 || countX>=5) {
        return 1; // Match nul si aucun joueur n'a 5 marqueurs
    }

    return 0; // Le jeu continue
}