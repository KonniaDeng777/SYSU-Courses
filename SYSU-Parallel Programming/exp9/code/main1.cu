#include <iostream>
#include <cuda_runtime.h>

__global__ void helloFromGPU(int m, int k) {
    int blockId = blockIdx.x;
    int threadIdX = threadIdx.x;
    int threadIdY = threadIdx.y;
    printf("Hello World from Thread (%d, %d) in Block %d!\n", threadIdX, threadIdY, blockId);
}

int main() {
    int n, m, k;
    std::cout << "Enter the values for n (number of blocks), m (threads in x dimension), k (threads in y dimension): ";
    std::cin >> n >> m >> k;
    dim3 threadsPerBlock(m, k);
    helloFromGPU<<<n, threadsPerBlock>>>(m, k);
    cudaDeviceSynchronize();
    std::cout << "Hello World from the host!" << std::endl;
    return 0;
}
