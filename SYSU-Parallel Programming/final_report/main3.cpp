/*
g++ -fopenmp main3.cpp -o main3
./main3

*/
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void multiplyMatrices(const vector<vector<int>> &A, const vector<vector<int>> &B, vector<vector<int>> &C) {
    int n = A.size();
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int n = 500;
    vector<vector<int>> A(n, vector<int>(n, 1));
    vector<vector<int>> B(n, vector<int>(n, 1));
    vector<vector<int>> C(n, vector<int>(n, 0));

    double start = omp_get_wtime();
    multiplyMatrices(A, B, C);
    double end = omp_get_wtime();

    cout << "Time taken: " << (end - start) * 1000 << " ms" << endl;

    return 0;
}
