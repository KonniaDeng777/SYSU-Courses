#include <iostream>
#include <iomanip>
#include <ctime>
#include "mkl.h"
using namespace std;

void multiply_matrices(int m, int n, int k, double* A, double* B, double* C) {
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, k, n, 1.0, A, n, B, k, 0.0, C, k);
}

void print_matrix(int rows, int cols, double* matrix) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i * cols + j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int m, n, k;
    cout << "Enter values for m, n, and k (between 512 and 2048): ";
    cin >> m >> n >> k;

    // Allocate memory for matrices
    double* A = new double[m * n];
    double* B = new double[n * k];
    double* C = new double[m * k];

    // Initialize matrices A and B with random values
    srand(time(0));
    for (int i = 0; i < m * n; i++) {
        A[i] = rand() / double(RAND_MAX);
    }
    for (int i = 0; i < n * k; i++) {
        B[i] = rand() / double(RAND_MAX);
    }

    // Perform matrix multiplication and measure the execution time
    clock_t start = clock();
    multiply_matrices(m, n, k, A, B, C);
    clock_t end = clock();
    double duration = double(end - start) / CLOCKS_PER_SEC;

    // Print matrices A, B, and C
    cout << "Matrix A:" << endl;
    print_matrix(m, n, A);
    cout << endl;

    cout << "Matrix B:" << endl;
    print_matrix(n, k, B);
    cout << endl;

    cout << "Matrix C:" << endl;
    print_matrix(m, k, C);
    cout << endl;

    // Print the execution time
    cout << "Running time: " << duration << " seconds" << endl;
    double totalFlops = 2 * static_cast<double>(m) * n * k; 
    double totalBytes = (m * n + n * k + m * k) * sizeof(double);  

    double flopsPerSecond = totalFlops / duration; 
    double bandwidth = totalBytes / duration;  

    double peakPerformancePercentage = (flopsPerSecond / bandwidth) * 100;  

    cout << "Floating Point Performance: " << fixed << setprecision(5) << flopsPerSecond << " FLOPS" << endl;
    cout << "Peak Bandwidth: " << fixed << setprecision(2) << bandwidth << " bytes/second" << endl;
    cout << "Peak Performance Percentage: " << fixed << setprecision(5) << peakPerformancePercentage << "%" << endl;

    // Deallocate memory
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}