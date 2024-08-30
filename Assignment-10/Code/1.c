#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 400
#define THREAD_COUNT 4

double matrix1[MATRIX_SIZE][MATRIX_SIZE];
double matrix2[MATRIX_SIZE][MATRIX_SIZE];
double result_matrix[MATRIX_SIZE][MATRIX_SIZE]={};
pthread_mutex_t result_matrix_lock;  // Mutex for synchronization

typedef struct {
    int start_row;
    int end_row;
} ThreadData;

void* matrix_multiply_thread(void* arg) {
    ThreadData* thread_data = (ThreadData*)arg;

    int start_row=thread_data->start_row,end_row=thread_data->end_row;

    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            double temp=0;
            for (int k = start_row; k < end_row; ++k) {
                temp+= matrix1[i][k] * matrix2[k][j];
            }
            pthread_mutex_lock(&result_matrix_lock);
            result_matrix[i][j]+=temp;
            pthread_mutex_unlock(&result_matrix_lock);
        }
    }

    pthread_exit(NULL);
}

int main() {

    FILE *f=fopen("write_matrix.txt","w");
    // Initialize matrices with random values
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            matrix1[i][j] = (double)rand() / RAND_MAX;
            matrix2[i][j] = (double)rand() / RAND_MAX;
        }
    }

    // Initialize mutex
    pthread_mutex_init(&result_matrix_lock, NULL);

    // Create threads
    pthread_t threads[THREAD_COUNT];
    ThreadData thread_data[THREAD_COUNT];

    // Divide rows among threads
    int rows_per_thread = MATRIX_SIZE / THREAD_COUNT;
    int cols_per_thread = MATRIX_SIZE / THREAD_COUNT;
    for (int i = 0; i < THREAD_COUNT; ++i) {
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i + 1) * rows_per_thread;
        if (i == THREAD_COUNT - 1) {
            thread_data[i].end_row = MATRIX_SIZE;
        }
        pthread_create(&threads[i], NULL, matrix_multiply_thread, &thread_data[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Print the result matrix
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            fprintf(f,"%lf ", result_matrix[i][j]);
        }
        fprintf(f,"\n");
    }

    // Destroy mutex
    pthread_mutex_destroy(&result_matrix_lock);

    fclose(f);

    return 0;
}
