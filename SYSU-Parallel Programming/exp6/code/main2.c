#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1024
#define NUM_THREADS 16

double plate[SIZE][SIZE];
double new_plate[SIZE][SIZE];
int iterations = 1024;

pthread_barrier_t barrier; 

struct for_index {
    int start;
    int end;
    int increment;
};

void* parallel_heat_conduction(void* arg) {
    struct for_index* index = (struct for_index*)arg;
    int start = index->start;
    int end = index->end;
    int increment = index->increment;

    for (int t = 0; t < iterations; t++) {
        for (int i = start; i < end; i += increment) {
            for (int j = 1; j < SIZE - 1; j++) {
                new_plate[i][j] = 0.25 * (plate[i - 1][j - 1] + plate[i - 1][j + 1] + plate[i + 1][j - 1] + plate[i + 1][j + 1]);
            }
        }

        pthread_barrier_wait(&barrier);

        for (int i = start; i < end; i += increment) {
            for (int j = 1; j < SIZE - 1; j++) {
                plate[i][j] = new_plate[i][j];
            }
        }

        pthread_barrier_wait(&barrier);
    }

    pthread_exit(NULL);
}

void parallel_for(int start, int end, int increment, void* (*functor)(void*), int num_threads) {
    pthread_t threads[num_threads];
    struct for_index index_arr[num_threads];

    int block = (end - start) / num_threads;

    for (int i = 0; i < num_threads; i++) {
        index_arr[i].start = start + i * block;
        index_arr[i].end = index_arr[i].start + block;
        if (i == (num_threads - 1))
            index_arr[i].end = end;
        index_arr[i].increment = increment;
        pthread_create(&threads[i], NULL, functor, (void*)&index_arr[i]);
    }

    for (int thread = 0; thread < num_threads; thread++)
        pthread_join(threads[thread], NULL);
}

int main() {
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            plate[i][j] = 0.0;
        }
    }

    for (int i = 0; i < SIZE; i++) {
        plate[i][0] = 1.0;
        plate[i][SIZE - 1] = 1.0;
        plate[0][i] = 1.0;
        plate[SIZE - 1][i] = 1.0;
    }

    parallel_for(1, SIZE - 1, 1, parallel_heat_conduction, NUM_THREADS);

    printf("Plate after %d iterations:\n", iterations);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%f ", plate[i][j]);
        }
        printf("\n");
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}
