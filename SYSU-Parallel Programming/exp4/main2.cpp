#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

#define NUM_THREADS 8

struct ThreadData {
    int totalPoints;
    int pointsInsideCircle;
};

void *monteCarlo(void *threadarg) {
    ThreadData *data;
    data = (ThreadData *) threadarg;

    int totalPoints = data->totalPoints;
    int pointsInsideCircle = 0;

    unsigned int seed = time(NULL);
    for (int i = 0; i < totalPoints; ++i) {
        double x = (double) rand_r(&seed) / RAND_MAX;
        double y = (double) rand_r(&seed) / RAND_MAX;

        if (x * x + y * y <= 1.0)
            pointsInsideCircle++;
    }

    data->pointsInsideCircle = pointsInsideCircle;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number of points>" << std::endl;
        return 1;
    }

    int totalPoints = atoi(argv[1]);
    if (totalPoints < 1024 || totalPoints > 65536) {
        std::cerr << "Number of points must be in the range [1024, 65536]" << std::endl;
        return 1;
    }

    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    int rc;
    long t;
    void *status;

    auto start = std::chrono::steady_clock::now();

    for (t = 0; t < NUM_THREADS; ++t) {
        threadData[t].totalPoints = totalPoints / NUM_THREADS;
        rc = pthread_create(&threads[t], NULL, monteCarlo, (void *) &threadData[t]);
        if (rc) {
            std::cerr << "Error" << rc << std::endl;
            exit(-1);
        }
    }

    for (t = 0; t < NUM_THREADS; ++t) {
        rc = pthread_join(threads[t], &status);
        if (rc) {
            std::cerr << "Error" << rc << std::endl;
            exit(-1);
        }
    }

    int totalInsideCircle = 0;
    for (t = 0; t < NUM_THREADS; ++t) {
        totalInsideCircle += threadData[t].pointsInsideCircle;
    }

    double pi = 4.0 * totalInsideCircle / totalPoints;

    auto end = std::chrono::steady_clock::now();


    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double timeElapsed = duration.count() / 1000000.0; 

    std::cout << "Total points: " << totalPoints << std::endl;
    std::cout << "Points inside circle: " << totalInsideCircle << std::endl;
    std::cout << "Approximated Pi: " << pi << std::endl;
    std::cout << "Running time: " << timeElapsed << " seconds" << std::endl;

    pthread_exit(NULL);
}
