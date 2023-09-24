/*

Programação Multithreaded para o Jogo da Vida - Versão OpenMP

Autor: Murilo Capozzi dos Santos
RA: 149425
Turma: I

*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BOARD_SIZE 2048
#define GENERATIONS 2000
#define NUM_THREADS 1

/* Iniciar a matriz zerada, com as duas primeira*/
float **initGrid(){
    float **erased_grid;
    int i, j ;

    erased_grid = malloc(BOARD_SIZE * sizeof (float*));

    for(i = 0; i < BOARD_SIZE; i++)
        erased_grid[i] = malloc(BOARD_SIZE * sizeof(float));

    for(i = 0; i < BOARD_SIZE; i++){
        for(j = 0; j < BOARD_SIZE; j++)
            erased_grid[i][j] = 0.0;
    }

    // GLIDER
    erased_grid[1][2] = 1.0;
    erased_grid[2][3] = 1.0;
    erased_grid[3][1] = 1.0;
    erased_grid[3][2] = 1.0;
    erased_grid[3][3] = 1.0;

    // R-PENTOMINO
    erased_grid[10][31] = 1.0;
    erased_grid[10][32] = 1.0;
    erased_grid[11][30] = 1.0;
    erased_grid[11][31] = 1.0;
    erased_grid[12][31] = 1.0;

    return erased_grid;
}

void freeGrid(float **grid){
    int i;
    for(i = 0; i < BOARD_SIZE; i++)
        free(grid[i]);
    free(grid);
}

/* Impressão da matriz */
void printGrid(float **grid){
    int i, j, quantity = 0;

    for(i = 0; i < 50; i++){
        for(j = 0; j < 50; j++){
            if(grid[i][j] > 0.0){
                printf("%.1f ", grid[i][j]);
            }
            else{
                printf("   ");
            }
        }
            printf("\n");
    }
    printf("----------------------\n\n");

}
 /* Quantificação de vivos */
int quantityAlive(float **grid){
    int i, j, quantity = 0;

    for(i = 0; i < BOARD_SIZE; i++){
        for(j = 0; j < BOARD_SIZE; j++){
            if(grid[i][j] > 0.0)
                quantity++;
        }
    }
    return quantity;
}

/* Simular a matriz de borda infinita */
int getCoord(int n){

    while(n % BOARD_SIZE < 0)
        n += BOARD_SIZE;

    return n % BOARD_SIZE;
}

/* Calcula a quantidade de vizinhos vivos */
int getNeighbors(float **grid, int i, int j){
    int k, l, quantity = 0;

    for(k = -1; k <= 1; k++){
        for(l = -1; l <= 1; l++){

            if(k == 0 && l == 0) // Ele mesmo
                continue;
            
            if(grid[getCoord(i + k)][getCoord(j + l)] > 0) // Vizinho vivo
                quantity++; 
        }
    }

    return quantity;
}

/* Calcula a media dos vizinhos */
float meanNeighbors(float **grid, int i, int j){
    int k, l;
    float sum = 0.0;

    for(k = -1; k <= 1; k++){
            for(l = -1; l <= 1; l++){

                if(k == 0 && l == 0) // Ele mesmo
                    continue;

                sum += grid[getCoord(i + k)][getCoord(j + l)];
            }
        }


    return (sum / 8.0);
}

/* Logica principal do jogo */
float** gameOfLife(float **grid, float **newgrid){

    int i, j, quantity = 0;
    float value;

#pragma omp parallel num_threads(NUM_THREADS) private(i,j,quantity,value) shared(grid, newgrid)

    for(i = 0; i < BOARD_SIZE; i++){

        for(j = 0; j < BOARD_SIZE; j++){

            quantity = getNeighbors(grid, i ,j);
            value = grid[i][j];

            // Regras para próximo estado
            if(value > 0 && (quantity == 2 || quantity == 3))     // Se estão vivas e possuem 2 ou 3 vizinhos, permanecem
                newgrid[i][j] = 1.0;
            else if(value == 0 && quantity == 3)                  // Se está morta e possui 3 vizinhos, renasce
                newgrid[i][j] = meanNeighbors(grid, i, j);
            else
                newgrid[i][j] = 0.0;
        }
    }

#pragma omp barrier // Sincronização
    return newgrid;
}

int main(int argc, char **argv){
    int i, quantity;

    float **actual = initGrid();
    float **new = initGrid();

    for(i = 0; i <= GENERATIONS; i++){

        float **temp = actual;
        actual = new;
        new = temp;

        #pragma omp single
            {
                if(i < 6 || i == GENERATIONS){
                    quantity = quantityAlive(actual);
                    printGrid(actual);
                    printf("Geração %d: %d celulas vivas\n", i, quantity);
                }
            }

        new = gameOfLife(actual, new);

        #pragma omp barrier
    }

    freeGrid(actual);
    freeGrid(new);

    return 0;
}