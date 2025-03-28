#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define mat_height 4096
#define mat_width 4096
#define filter_height 3
#define filter_width 3
#define stride 3

#define block_size_x 32
#define block_size_y 32

#define GET_TIME(now)                           \
    {                                           \
        struct timeval t;                       \
        gettimeofday(&t, NULL);                 \
        now = t.tv_sec + t.tv_usec / 1000000.0; \
    }

int check(float *c, float *d, int n);

#define padding_height ((filter_height / 2) * 2)
#define padding_width ((filter_width / 2) * 2)
#define input_height (mat_height + padding_height)
#define input_width (mat_width + padding_width)

void cpu_convolution(float *output, float *input, float *filter) {
    for (int y = 0; y < mat_height; y += stride) {
        for (int x = 0; x < mat_width; x += stride) {
            for (int i = 0; i < filter_height; i++) {
                for (int j = 0; j < filter_width; j++) {
                    output[y / stride * mat_width + x / stride] += input[(y + i) * input_width + x + j] * filter[i * filter_width + j];
                }
            }
        }
    }
}

void cpu_add(float *arr1, float *arr2, float *arr3, float *result) {
    for (int y = 0; y < mat_height; y += stride) {
        for (int x = 0; x < mat_width; x += stride) {
            int temp = y / stride * mat_width + x / stride;
            result[temp] = arr1[temp] + arr2[temp] + arr3[temp];
        }
    }
}

__global__ void cuda_convolution(float *output, float *input, float *filter) {
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    float sum = 0.0f;
    if (y % stride == 0 && x % stride == 0) {
        for (int i = 0; i < filter_height; i++) {
            for (int j = 0; j < filter_width; j++) {
                sum += input[(y + i) * input_width + x + j] * filter[i * filter_width + j];
            }
        }
        output[y / stride * mat_width + x / stride] = sum;
    }
}

__global__ void cuda_add(float *arr1, float *arr2, float *arr3, float *result) {
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    if (y % stride == 0 && x % stride == 0) {
        result[y / stride * mat_width + x / stride] = arr1[y / stride * mat_width + x / stride] + arr2[y / stride * mat_width + x / stride] + arr3[y / stride * mat_width + x / stride];
    }
}

void init_matrix(float *input, float *filter) {
    for (int i = 0; i < input_height * input_width; i++) {
        input[i] = (float)(rand() % 50) / 100;
    }
    for (int i = 0; i < filter_height * filter_width; i++) {
        filter[i] = (float)(rand() % 50) / 100;
    }
}

int main() {
    double start, finish, time1, time2;

    float *inputs[3], *outputs_cpu[3], *outputs_cuda[3], *filters[3], *result_cpu, *result_cuda;
    for (int i = 0; i < 3; i++) {
        inputs[i] = (float *)malloc(input_height * input_width * sizeof(float));
        outputs_cpu[i] = (float *)malloc(mat_height * mat_width * sizeof(float));
        outputs_cuda[i] = (float *)malloc(mat_height * mat_width * sizeof(float));
        filters[i] = (float *)malloc(filter_height * filter_width * sizeof(float));
        init_matrix(inputs[i], filters[i]);
        memset(outputs_cpu[i], 0, mat_height * mat_width * sizeof(float));
        memset(outputs_cuda[i], 0, mat_height * mat_width * sizeof(float));
    }
    result_cpu = (float *)malloc(mat_height * mat_width * sizeof(float));
    result_cuda = (float *)malloc(mat_height * mat_width * sizeof(float));

    GET_TIME(start);
    for (int i = 0; i < 3; i++) {
        cpu_convolution(outputs_cpu[i], inputs[i], filters[i]);
    }
    cpu_add(outputs_cpu[0], outputs_cpu[1], outputs_cpu[2], result_cpu);
    GET_TIME(finish);
    time1 = finish - start;
    printf("Sequential convolution time:  %f s\n", time1);

    float *d_inputs[3], *d_outputs[3], *d_filters[3], *d_result;
    for (int i = 0; i < 3; i++) {
        cudaMalloc((void **)&d_inputs[i], input_height * input_width * sizeof(float));
        cudaMalloc((void **)&d_outputs[i], mat_height * mat_width * sizeof(float));
        cudaMalloc((void **)&d_filters[i], filter_height * filter_width * sizeof(float));
        cudaMemcpy(d_inputs[i], inputs[i], input_height * input_width * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(d_filters[i], filters[i], filter_height * filter_width * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemset(d_outputs[i], 0, mat_height * mat_width * sizeof(float));
    }
    cudaMalloc((void **)&d_result, mat_height * mat_width * sizeof(float));

    dim3 threads(block_size_x, block_size_y);
    dim3 grid((mat_width + threads.x - 1) / threads.x, (mat_height + threads.y - 1) / threads.y);

    cudaDeviceSynchronize();
    GET_TIME(start);
    for (int i = 0; i < 3; i++) {
        cuda_convolution<<<grid, threads>>>(d_outputs[i], d_inputs[i], d_filters[i]);
    }
    cuda_add<<<grid, threads>>>(d_outputs[0], d_outputs[1], d_outputs[2], d_result);
    cudaDeviceSynchronize();
    GET_TIME(finish);
    time2 = finish - start;
    printf("CUDA convolution time:        %f s\n", time2);

    cudaMemcpy(result_cuda, d_result, mat_height * mat_width * sizeof(float), cudaMemcpyDeviceToHost);

    FILE *fp = fopen("result.txt", "w");
    for (int y = 0; y < mat_height; y += stride) {
        for (int x = 0; x < mat_width; x += stride) {
            fprintf(fp, "%f ", result_cuda[y / stride * mat_width + x / stride]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);

    printf("Running time:      %f s\n",time1+time2);

    if (check(result_cpu, result_cuda, mat_height * mat_width) > 0) {
        printf("The result is wrong.\n");
    } else {
        printf("The result is right.\n");
    }

    for (int i = 0; i < 3; i++) {
        cudaFree(d_inputs[i]);
        cudaFree(d_outputs[i]);
        cudaFree(d_filters[i]);
        free(inputs[i]);
        free(outputs_cpu[i]);
        free(outputs_cuda[i]);
        free(filters[i]);
    }
    cudaFree(d_result);
    free(result_cpu);
    free(result_cuda);

    return 0;
}

int check(float *arr1, float *arr2, int n) {
    int errors = 0;
    for (int y = 0; y < mat_height; y += stride) {
        for (int x = 0; x < mat_width; x += stride) {
            int i = y / stride * mat_width + x / stride;
            if (isnan(arr1[i]) || isnan(arr2[i]))
                errors++;
            float diff = (arr1[i] - arr2[i]) / arr1[i];
            if (diff > 1e-6f)
                errors++;
        }
    }
    return errors;
}
