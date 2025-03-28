/*
nvcc main4.cu -o main4
./main4
*/
#include <iostream>
#include <vector>
#include <cuda.h>

using namespace std;

__global__ void multiplyMatricesKernel(int *A, int *B, int *C, int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < n && col < n) {
        int sum = 0;
        for (int k = 0; k < n; ++k) {
            sum += A[row * n + k] * B[k * n + col];
        }
        C[row * n + col] = sum;
    }
}

void multiplyMatricesCUDA(const vector<vector<int>> &A, const vector<vector<int>> &B, vector<vector<int>> &C) {
    int n = A.size();
    int size = n * n * sizeof(int);

    int *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    cudaMemcpy(d_A, &A[0][0], size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, &B[0][0], size, cudaMemcpyHostToDevice);

    dim3 threadsPerBlock(16, 16);
    dim3 blocksPerGrid((n + 15) / 16, (n + 15) / 16);

    multiplyMatricesKernel<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, n);

    cudaMemcpy(&C[0][0], d_C, size, cudaMemcpyDeviceToHost);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
}

int main() {
    int n = 500;
    vector<vector<int>> A(n, vector<int>(n, 1));
    vector<vector<int>> B(n, vector<int>(n, 1));
    vector<vector<int>> C(n, vector<int>(n, 0));

    multiplyMatricesCUDA(A, B, C);

    return 0;
}
