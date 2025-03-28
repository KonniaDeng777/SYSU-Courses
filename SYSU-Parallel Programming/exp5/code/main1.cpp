#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <omp.h>

using namespace std;

vector<vector<double>> generateRandomMatrix(int rows, int cols) {
    vector<vector<double>> matrix(rows, vector<double>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = rand() % 100; 
        }
    }
    return matrix;
}

vector<vector<double>> matrixMultiplication(const vector<vector<double>>& A,
                                            const vector<vector<double>>& B) {
    int m = A.size();
    int n = A[0].size();
    int k = B[0].size();
    vector<vector<double>> C(m, vector<double>(k, 0.0));
    
    #pragma omp parallel for
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < k; ++j) {
            for (int p = 0; p < n; ++p) {
                C[i][j] += A[i][p] * B[p][j];
            }
        }
    }
    
    return C;
}

int main() {
    srand(time(0)); 
    
    const int minSize = 128;
    const int maxSize = 1024;
    const int minThreads = 1;
    const int maxThreads = 16;
    const char* schedules[] = {"default", "static", "dynamic"};
    const int numSchedules = 3;
    
    for (int threads = minThreads; threads <= maxThreads; threads*=2) {
        for (int size = minSize; size <= maxSize; size *= 2) {
            for (int sched = 0; sched < numSchedules; ++sched) {
                vector<vector<double>> A = generateRandomMatrix(size, size);
                vector<vector<double>> B = generateRandomMatrix(size, size);
                
                auto start = chrono::steady_clock::now();
                
                omp_set_num_threads(threads);
                omp_set_schedule(omp_sched_dynamic, 0); //default
                if (sched == 1)
                    omp_set_schedule(omp_sched_static, 0); //static 
                else if (sched == 2)
                    omp_set_schedule(omp_sched_dynamic, 0); //dynamic 
                
                vector<vector<double>> C = matrixMultiplication(A, B);
                
                auto end = chrono::steady_clock::now();
                auto diff = end - start;
                double elapsedTime = chrono::duration <double, milli>(diff).count();
                
                cout << "Threads: " << threads << ", Matrix Size: " << size << "x" << size
                     << ", Schedule: " << schedules[sched] << ", Running Time: " << elapsedTime << " ms" << endl;
            }
        }
    }
    
    return 0;
}
