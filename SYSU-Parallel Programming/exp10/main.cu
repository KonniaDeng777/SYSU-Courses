#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <chrono>
#include <cstdlib>

// CUDA核函数用于矩阵乘法
__global__ void matrixMulKernel(float* A, float* B, float* C, int m, int n, int k) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < m && col < k) {
        float value = 0;
        for (int e = 0; e < n; ++e) {
            value += A[row * n + e] * B[e * k + col];
        }
        C[row * k + col] = value;
    }
}

// 矩阵初始化函数
void initializeMatrix(float* matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = static_cast<float>(rand()) / RAND_MAX;
    }
}

// 矩阵乘法主函数
void matrixMulCUDA(int m, int n, int k, int blockSize) {
    size_t sizeA = m * n * sizeof(float);
    size_t sizeB = n * k * sizeof(float);
    size_t sizeC = m * k * sizeof(float);

    // 分配主机内存
    float* h_A = (float*)malloc(sizeA);
    float* h_B = (float*)malloc(sizeB);
    float* h_C = (float*)malloc(sizeC);

    // 初始化矩阵A和B
    initializeMatrix(h_A, m, n);
    initializeMatrix(h_B, n, k);

    // 分配设备内存
    float* d_A; cudaMalloc(&d_A, sizeA);
    float* d_B; cudaMalloc(&d_B, sizeB);
    float* d_C; cudaMalloc(&d_C, sizeC);

    // 将主机内存数据拷贝到设备内存
    cudaMemcpy(d_A, h_A, sizeA, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, sizeB, cudaMemcpyHostToDevice);

    // 定义CUDA网格和块结构
    dim3 blockDim(blockSize, blockSize);
    dim3 gridDim((k + blockSize - 1) / blockSize, (m + blockSize - 1) / blockSize);

    // 记录开始时间
    auto start = std::chrono::high_resolution_clock::now();

    // 调用矩阵乘法核函数
    matrixMulKernel<<<gridDim, blockDim>>>(d_A, d_B, d_C, m, n, k);

    // 同步设备
    cudaDeviceSynchronize();

    // 记录结束时间
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = end - start;

    // 将结果从设备内存拷贝回主机内存
    cudaMemcpy(h_C, d_C, sizeC, cudaMemcpyDeviceToHost);

    // 输出计算时间
    std::cout << "Running time: " << duration.count() << " ms" << std::endl;

    // 释放内存
    free(h_A);
    free(h_B);
    free(h_C);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
}

int main() {
    int m, n, k, blockSize;

    // 输入矩阵的维度和线程块大小
    std::cout << "Enter m,n,k: ";
    std::cin >> m;
    std::cin >> n;
    std::cin >> k;
    std::cout << "Enter the block size: ";
    std::cin >> blockSize;

    // 运行矩阵乘法
    matrixMulCUDA(m, n, k, blockSize);

    return 0;
}
