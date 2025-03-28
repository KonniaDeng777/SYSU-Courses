#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

#define MAX_THREADS 16
using namespace std;

struct ThreadData {
    int* array;
    int start;
    int end;
    long long sum;
};

void* sumArray(void* threadArg) {
    ThreadData* data = (ThreadData*)threadArg;
    long long sum = 0;
    for (int i = data->start; i < data->end; ++i) {
        sum += data->array[i];
    }
    data->sum = sum;
    pthread_exit(NULL);
}

int main() 
{
    srand(time(NULL));
    int num_threads;
    int n;
    cout << "Enter the size of the array (between 1M and 128M): ";
    cin >> n;
    cout<<"Enter number of threads:";
    cin>>num_threads;
    if (n < 1e6 || n > 128e6) {
        cerr << "Array size must be between 1M and 128M." << endl;
        return 1;
    }
    
    int* array = new int[n];

    for (int i = 0; i < n; ++i) 
    {
        array[i] = rand() % 100; 
    }

    std::cout << "Array:" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    clock_t start = clock();
    pthread_t threads[MAX_THREADS];
    ThreadData threadData[MAX_THREADS];
    int chunkSize = n / num_threads;
    for (int i = 0; i < num_threads; ++i) {
        threadData[i].array = array;
        threadData[i].start = i * chunkSize;
        threadData[i].end = (i == num_threads - 1) ? n : (i + 1) * chunkSize;
        pthread_create(&threads[i], NULL, sumArray, (void*)&threadData[i]);
    }

    long long totalSum = 0;
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
        totalSum += threadData[i].sum;
    }

    clock_t end = clock();
    double duration = double(end - start) / CLOCKS_PER_SEC;

    cout << "Total sum: " << totalSum << endl;
    cout << "Running time: " << duration<< " seconds" << endl;

    delete[] array;

    return 0;
}
