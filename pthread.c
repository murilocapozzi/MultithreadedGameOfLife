/*

Programação Multithreaded para o Jogo da Vida - Versão Pthread

Autor: Murilo Capozzi dos Santos
RA: 149425
Turma: I

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BOARD_SIZE 50
#define GENERATIONS 50
#define NUM_THREADS 8


pthread_t t[NUM_THREADS];
int i = 0;

typedef struct {
    int t_id;
    int initial;
    int final;
    float **actual;
    float **new;
    int quantity;
}thread_data;

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

int quantityAlive(float **grid){
    int i, j, quantity = 0;

    for(i = 0; i < BOARD_SIZE; i++){
        for(j = 0; j < BOARD_SIZE; j++){
            if(grid[i][j] > 0.0){
                quantity++;
                printf("%.1f ", grid[i][j]);
            }
            else{
                printf("   ");
            }
        }
            printf("\n");
    }
    printf("----------------------\n\n");

    return quantity;
}

// Simular a matriz de borda infinita
int getCoord(int n){

    while(n % BOARD_SIZE < 0)
        n += BOARD_SIZE;

    return n % BOARD_SIZE;
}

int getNeighbors(float **grid, int i, int j){
    int k, l, quantity = 0;

    for(k = -1; k <= 1; k++){
        for(l = -1; l <= 1; l++){
            if(k == 0 && l == 0)
                continue;
            
            if(grid[getCoord(i + k)][getCoord(j + l)] > 0)
                quantity++; 
        }
    }

    return quantity;
}

float meanNeighbors(float **grid, int i, int j){
    int k, l;
    float sum = 0.0;

    for(k = -1; k <= 1; k++){
            for(l = -1; l <= 1; l++){
                if(k == 0 && l == 0)
                    continue;

                sum += grid[getCoord(i + k)][getCoord(j + l)];
            }
        }


    return (sum / 8.0);
}

void *gameOfLife_pthread(void *arg){

    thread_data *tdata=(thread_data *)arg;

    int i, j, quantity;
    float value;

    int *quantity_alive = malloc(sizeof(int));
    *quantity_alive = 0;

    for(i = tdata->initial; i <= tdata->final; i++){

        for(j = 0; j < BOARD_SIZE; j++){

            quantity = getNeighbors(tdata->actual, i ,j);
            value = tdata->actual[i][j];

            // Regras para próximo estado
            if(value > 0 && (quantity == 2 || quantity == 3)){     // Se estão vivas e possuem 2 ou 3 vizinhos, permanecem
                tdata->new[i][j] = 1.0;
                (*quantity_alive)++;
            }
            else if(value == 0 && quantity == 3){           // Se está morta e possui 3 vizinhos, renasce
                tdata->new[i][j] = meanNeighbors(tdata->actual, i, j);
                (*quantity_alive)++;
            }
            else
                tdata->new[i][j] = 0.0;
        }
    }

    tdata->quantity = *quantity_alive;

    pthread_exit((void *)tdata);
}

int main(int argc, char **argv){
    int j, quantity;
    char c;

    float **actual = initGrid();
    float **new = initGrid();


    for(i = 1; i <= GENERATIONS; i++){

        float **temp = actual;
        actual = new;
        new = temp;

        for(j = 0; j < NUM_THREADS; j++){
            thread_data *tdata = malloc(sizeof(thread_data));
            tdata->t_id = j;
            tdata->actual = actual;
            tdata->new = new;

            /* Parcela a matriz igualmente para cada thread */
            tdata->initial = BOARD_SIZE * j / NUM_THREADS;
            tdata->final = BOARD_SIZE * (j + 1) / NUM_THREADS - 1;

            pthread_create(&t[j], NULL, gameOfLife_pthread, (void *)tdata);

        }

        for(j = 0, quantity = 0; j < NUM_THREADS; j++){
            thread_data *tdata = malloc(sizeof(thread_data));

            pthread_join(t[j], (void *)&tdata);

            quantity += tdata->quantity;

        }
        if(i < 6 || i == GENERATIONS){
            int quantity = quantityAlive(actual);
            printf("Geração %d: %d celulas vivas\n", i, quantity);
            scanf("%c", &c);
        }
    }

    freeGrid(actual);
    freeGrid(new);

    return 0;
}