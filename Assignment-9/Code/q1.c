#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 9  // Sudoku grid size
#define NUM_THREADS 11  // Number of threads: 9 for rows, 9 for columns, 1 for subgrid

typedef struct{
    int row;
    int column;
}parameters;

typedef struct{
    parameters param;
    int index;
} args;

int sudokuGrid[N][N];

int results[NUM_THREADS] = {0};
pthread_t threads[NUM_THREADS];

// Function to check if a 3x3 subgrid is valid
void* checkSubgrid(void* arg) {
    args* arg1 = (args*)arg;
    int row = arg1->param.row;
    int col = arg1->param.column;
    int subgridIndex = arg1->index;

    int presence[N] = {0};

    for (int i = row; i < row + 3; i++) {
        for (int j = col; j < col + 3; j++) {
            int num = sudokuGrid[i][j];
            if (num < 1 || num > N || presence[num - 1]) {
                results[subgridIndex] = 0;  // Invalid subgrid
                pthread_exit(NULL);
            }
            presence[num - 1] = 1;
        }
    }
    results[subgridIndex] = 1;  // Valid subgrid
    pthread_exit(NULL);
}

// Function to check if a row is valid
void* checkRow(void* arg) {
    args* arg1 = (args*)arg;
    int row = arg1->param.row;
    int rowIndex = arg1->index;
    
    for(int i=row;i<N;i++)
    {
        int presence[N] = {0};

        for (int j = 0; j < N; j++) {
            int num = sudokuGrid[i][j];
            if (num < 1 || num > N || presence[num - 1]) {
                results[rowIndex] = 0;  // Invalid row
                pthread_exit(NULL);   
            }
            presence[num - 1] = 1;
        }
    }
    results[rowIndex] = 1;  // Valid row
    pthread_exit(NULL);
}

// Function to check if a column is valid
void* checkColumn(void* arg) {

    args* arg1 = (args*)arg;
    int col = arg1->param.column;
    int colIndex = arg1->index;

    for(int j=col;j<N;j++)
    {
        int presence[N] = {0};

        for (int i = 0; i < N; i++) {
            int num = sudokuGrid[i][j];
            if (num < 1 || num > N || presence[num - 1]) {
                results[colIndex] = 0;  // Invalid column
                pthread_exit(NULL);
            }
            presence[num - 1] = 1;
        }
    }
    results[colIndex] = 1;  // Valid column
    pthread_exit(NULL);
}

int main() {
    int i,j;

    FILE *f=fopen("week10-ML2-input1.txt","r");

    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            fscanf(f,"%d",&sudokuGrid[i][j]);
        }
    }

    // Create threads to check rows
    args* arg=(args*)malloc(sizeof(args));
    arg->param.row=0;
    arg->index=0;
    pthread_create(&threads[0], NULL, checkRow, (void*)arg);        

    // Create threads to check columns
    args* arg1=(args*)malloc(sizeof(args));
    arg1->param.column=0;
    arg1->index=1;
    pthread_create(&threads[1], NULL, checkColumn, (void *)arg1);

    // Create threads to check 3x3 subgrids
    for (i = 0; i < 9; i++) {
        args* arg=(args*)malloc(sizeof(args));
        int row = (i / 3) * 3;
        int col = (i % 3) * 3;
        arg->param.row=row;
        arg->param.column=col;
        arg->index=2+i;
        pthread_create(&threads[2+ i], NULL, checkSubgrid, (void *)arg);
    }

    for(int i=0;i<NUM_THREADS;i++)
    {
        pthread_join(threads[i],NULL);
    }

    // Check the results to determine if the Sudoku puzzle is valid
    int valid = 1;
    for (i = 0; i < NUM_THREADS; i++) {
        if (results[i] == 0) {
            valid = 0;
            break;
        }
    }

    if (valid) {
        printf("Sudoku puzzle is valid.\n");
    } else {
        printf("Sudoku puzzle is not valid.\n");
    }

    return 0;
}
