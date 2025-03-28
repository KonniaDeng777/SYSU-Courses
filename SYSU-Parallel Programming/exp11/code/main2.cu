#include <stdio.h>
#include <sys/time.h>

#define HEIGHT 256
#define WIDTH 256
#define FILTER_SIZE 3
#define STRIDE 1
#define PAD 1
#define CHANNELS 3

#define BLOCK_SIZE_X 32
#define BLOCK_SIZE_Y 32

#define GET_TIME(now)                           \
    {                                           \
        struct timeval t;                       \
        gettimeofday(&t, NULL);                 \
        now = t.tv_sec + t.tv_usec / 1000000.0; \
    }

// 并行矩阵乘法函数
__global__ void matMultCUDA(const float *a, const float *b, float *c, int M, int N, int K, int BLOCK_SIZE) {
    int tid = threadIdx.x;
    int bid = blockIdx.x;
    int idx = bid * BLOCK_SIZE + tid;
    int row = idx / M;
    int column = idx % M;
    while (column < K) {
        if (row < M && column < K) {
            float t = 0;
            for (int i = 0; i < N; i++) {
                t += a[row * N + i] * b[i * K + column];
            }
            c[row * K + column] = t;
        }
        column += M;
    }
}

float im2col_get_data(float *im, int row, int col, int channel) { 
    row -= PAD;
    col -= PAD;
    if (row < 0 || col < 0 || row >= HEIGHT || col >= WIDTH) return 0;
    return im[col + WIDTH * (row + HEIGHT * channel)];
}
    
void im2col(float *data_im, float *data_col) {   
    int height_col = (HEIGHT + 2 * PAD - FILTER_SIZE) / STRIDE + 1;    
    int width_col = (WIDTH + 2 * PAD - FILTER_SIZE) / STRIDE + 1;
    int channels_col = CHANNELS * FILTER_SIZE * FILTER_SIZE;    
    for (int c = 0; c < channels_col; ++c) {
        int w_offset = c % FILTER_SIZE;        
        int h_offset = (c / FILTER_SIZE) % FILTER_SIZE;        
        int c_im = c / FILTER_SIZE / FILTER_SIZE;        
        for (int h = 0; h < height_col; ++h) {
            for (int w = 0; w < width_col; ++w) { 
                int im_row = h_offset + h * STRIDE;                
                int im_col = w_offset + w * STRIDE;                
                int col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = im2col_get_data(data_im, im_row, im_col, c_im);
            }
        }
    }
}

int main() {
    double start, finish, time;
    int height_col = (HEIGHT + 2 * PAD - FILTER_SIZE) / STRIDE + 1;    
    int width_col = (WIDTH + 2 * PAD - FILTER_SIZE) / STRIDE + 1;    
    int channels_col = CHANNELS * FILTER_SIZE * FILTER_SIZE;    
    
    float *im = (float *)malloc(HEIGHT * WIDTH * CHANNELS * sizeof(float));
    float *col = (float *)malloc(channels_col * height_col * width_col * sizeof(float));
    float *filter = (float *)malloc(CHANNELS * FILTER_SIZE * FILTER_SIZE * sizeof(float));
    float *c = (float *)malloc(CHANNELS * width_col * height_col * sizeof(float));

    for (int i = 0; i < HEIGHT * WIDTH * CHANNELS; i++) {
        im[i] = (float)(rand() % 50) / 100;
    }
    for (int i = 0; i < FILTER_SIZE * FILTER_SIZE * CHANNELS; i++) {
        filter[i] = (float)(rand() % 50) / 100;
    }

    GET_TIME(start);

    im2col(im, col);

    float *cuda_a, *cuda_b, *cuda_c;
    cudaMalloc((void **)&cuda_a, sizeof(float) * CHANNELS * FILTER_SIZE * FILTER_SIZE);
    cudaMalloc((void **)&cuda_b, sizeof(float) * channels_col * height_col * width_col);
    cudaMalloc((void **)&cuda_c, sizeof(float) * CHANNELS * width_col * height_col);

    cudaMemcpy(cuda_a, filter, sizeof(float) * CHANNELS * FILTER_SIZE * FILTER_SIZE, cudaMemcpyHostToDevice);
    cudaMemcpy(cuda_b, col, sizeof(float) * channels_col * height_col * width_col, cudaMemcpyHostToDevice);

    int BLOCK_SIZE = HEIGHT;
    int blocks_num = (CHANNELS * width_col * height_col + BLOCK_SIZE - 1) / BLOCK_SIZE;

    matMultCUDA<<<blocks_num, BLOCK_SIZE>>>(cuda_a, cuda_b, cuda_c, CHANNELS, FILTER_SIZE * FILTER_SIZE, width_col * height_col, BLOCK_SIZE);

    cudaMemcpy(c, cuda_c, sizeof(float) * CHANNELS * width_col * height_col, cudaMemcpyDeviceToHost);

    GET_TIME(finish);
    time = finish - start;

    FILE *fp = fopen("result.txt", "w");
    for (int i = 0; i < CHANNELS * width_col * height_col; i++) {
        fprintf(fp, "%f ", c[i]);
    }
    fclose(fp);

    cudaFree(cuda_a);
    cudaFree(cuda_b);
    cudaFree(cuda_c);
    free(c);
    free(im);
    free(col);

    printf("Running time: %f s\n\n", time);

    return 0;
}
