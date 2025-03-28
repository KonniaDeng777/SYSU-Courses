#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cuda_runtime.h>

__global__ void transpose(int *A, int *AT, int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < n && col < n) {
        AT[row * n + col] = A[col * n + row];
    }
}

int main() {
    int n;
    std::cout << "Enter the size of the square matrix (between 512 and 2048): ";
    std::cin >> n;

    if (n < 512 || n > 2048) {
        std::cerr << "Size must be between 512 and 2048!" << std::endl;
        return 1;
    }

    int *h_A = new int[n * n];
    int *h_AT = new int[n * n];

    srand(time(NULL));
    for (int i = 0; i < n * n; ++i) {
        h_A[i] = rand() % 100; 
    }

    int *d_A, *d_AT;
    cudaMalloc((void**)&d_A, n * n * sizeof(int));
    cudaMalloc((void**)&d_AT, n * n * sizeof(int));

    cudaMemcpy(d_A, h_A, n * n * sizeof(int), cudaMemcpyHostToDevice);

    int block_size;
    std::cout << "Enter the block size for CUDA kernel: ";
    std::cin >> block_size;
    dim3 blockSize(block_size, block_size);
    dim3 gridSize((n + blockSize.x - 1) / blockSize.x, (n + blockSize.y - 1) / blockSize.y);

    clock_t start = clock();
    transpose<<<gridSize, blockSize>>>(d_A, d_AT, n);
    cudaDeviceSynchronize();
    clock_t end = clock();
    double time_taken = double(end - start) / CLOCKS_PER_SEC;

    cudaMemcpy(h_AT, d_AT, n * n * sizeof(int), cudaMemcpyDeviceToHost);

    std::cout << "Matrix A:" << std::endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << h_A[i * n + j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Transpose of A :" << std::endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << h_AT[i * n + j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Running time : " << time_taken << " seconds" << std::endl;

    delete[] h_A;
    delete[] h_AT;
    cudaFree(d_A);
    cudaFree(d_AT);

    return 0;
}
