#include<stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include<iostream>
#include"parallel_for.h"

void parallel_for(int start, int end, int increment, void *(*functor)(void *), void *arg, int num_threads){
    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    for_index *index_arr = (for_index *)malloc(num_threads * sizeof(for_index));

    int block = (end - start) / num_threads;

    for (int i = 0; i < num_threads; i++){
        index_arr[i].args = arg;
        index_arr[i].start = start + i * block;
        index_arr[i].end = index_arr[i].start + block;
        if (i == (num_threads - 1))
            index_arr[i].end = end;
        index_arr[i].increment = increment;
        pthread_create(&threads[i], NULL, functor, (void *)(index_arr + i));
    }
    for (int thread = 0; thread < num_threads; thread++)
        pthread_join(threads[thread], NULL);
    free(threads);
    free(index_arr);
}
