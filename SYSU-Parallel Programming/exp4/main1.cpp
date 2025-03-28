#include <iostream>
#include <pthread.h>
#include <cmath>
#include <ctime>

using namespace std;

const int NUM_THREADS = 4; 

struct ThreadData {
    double a, b, c;
    double x1, x2;
};

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int threads_completed = 0;

void* quadraticEquation(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double a = data->a;
    double b = data->b;
    double c = data->c;

    double discriminant = b * b - 4 * a * c;
    if (discriminant >= 0) {
        data->x1 = (-b + sqrt(discriminant)) / (2 * a);
        data->x2 = (-b - sqrt(discriminant)) / (2 * a);
    }

    pthread_mutex_lock(&mutex1);
    threads_completed++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

int main() {
    double a, b, c;
    cout << "Enter coefficients (a, b, c): ";
    cin >> a >> b >> c;

    ThreadData data;
    data.a = a;
    data.b = b;
    data.c = c;

    pthread_t threads[NUM_THREADS];
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, quadraticEquation, (void*)&data);
    }

    pthread_mutex_lock(&mutex1);
    while (threads_completed < NUM_THREADS) {
        pthread_cond_wait(&cond, &mutex1);
    }
    pthread_mutex_unlock(&mutex1);

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    cout << "x1 = " << data.x1 << ", x2 = " << data.x2 << endl;
    cout << "Running time: " << cpu_time_used << " seconds" << endl;

    return 0;
}
