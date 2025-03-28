#include <cudnn.h>
#include <cuda.h>
#include <iostream>
using namespace std;

#define HEIGHT 4096
#define WIDTH 4096
#define FILTER 3
#define STRIDE 3
#define PAD 1
#define CHANNELS 3
#define OUT_HEIGHT (HEIGHT + 2 * PAD - FILTER) / STRIDE + 1
#define OUT_WIDTH (WIDTH + 2 * PAD - FILTER) / STRIDE + 1

#define checkCUDNN(expression)                                     \
    {                                                              \
        cudnnStatus_t status = (expression);                       \
        if (status != CUDNN_STATUS_SUCCESS)                        \
        {                                                          \
            std::cerr << "Error on line " << __LINE__ << ": "      \
                      << cudnnGetErrorString(status) << std::endl; \
            std::exit(EXIT_FAILURE);                               \
        }                                                          \
    }

int main()
{
    cudnnHandle_t cudnn;
    cudnnCreate(&cudnn);

    // Initialize input image
    float *image = (float *)malloc(HEIGHT * WIDTH * CHANNELS * sizeof(float));
    for (int i = 0; i < HEIGHT * WIDTH * CHANNELS; ++i)
        image[i] = (float)(rand() % 50) / 100;

    // Tensor descriptors
    cudnnTensorDescriptor_t input_descriptor, output_descriptor;
    cudnnFilterDescriptor_t kernel_descriptor;
    cudnnConvolutionDescriptor_t convolution_descriptor;

    checkCUDNN(cudnnCreateTensorDescriptor(&input_descriptor));
    checkCUDNN(cudnnSetTensor4dDescriptor(input_descriptor, CUDNN_TENSOR_NHWC, CUDNN_DATA_FLOAT, 1, CHANNELS, HEIGHT, WIDTH));

    checkCUDNN(cudnnCreateTensorDescriptor(&output_descriptor));
    checkCUDNN(cudnnSetTensor4dDescriptor(output_descriptor, CUDNN_TENSOR_NHWC, CUDNN_DATA_FLOAT, 1, 1, OUT_HEIGHT, OUT_WIDTH));

    checkCUDNN(cudnnCreateFilterDescriptor(&kernel_descriptor));
    checkCUDNN(cudnnSetFilter4dDescriptor(kernel_descriptor, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, 1, CHANNELS, FILTER, FILTER));

    checkCUDNN(cudnnCreateConvolutionDescriptor(&convolution_descriptor));
    checkCUDNN(cudnnSetConvolution2dDescriptor(convolution_descriptor, PAD, PAD, STRIDE, STRIDE, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT));

    cudnnConvolutionFwdAlgo_t convolution_algorithm;
    checkCUDNN(cudnnGetConvolutionForwardAlgorithm(cudnn, input_descriptor, kernel_descriptor, convolution_descriptor, output_descriptor, CUDNN_CONVOLUTION_FWD_PREFER_FASTEST, 0, &convolution_algorithm));

    size_t workspace_bytes = 0;
    checkCUDNN(cudnnGetConvolutionForwardWorkspaceSize(cudnn, input_descriptor, kernel_descriptor, convolution_descriptor, output_descriptor, convolution_algorithm, &workspace_bytes));

    // Memory allocation
    void *d_workspace{nullptr};
    cudaMalloc(&d_workspace, workspace_bytes);

    float *d_input{nullptr}, *d_output{nullptr}, *d_kernel{nullptr};
    size_t image_bytes = HEIGHT * WIDTH * CHANNELS * sizeof(float);

    cudaMalloc(&d_input, image_bytes);
    cudaMemcpy(d_input, image, image_bytes, cudaMemcpyHostToDevice);

    cudaMalloc(&d_output, image_bytes);
    cudaMemset(d_output, 0, image_bytes);

    float h_kernel[FILTER][CHANNELS][FILTER][FILTER];
    for (int kernel = 0; kernel < FILTER; ++kernel)
        for (int channel = 0; channel < CHANNELS; ++channel)
            for (int row = 0; row < FILTER; ++row)
                for (int col = 0; col < FILTER; ++col)
                    h_kernel[kernel][channel][row][col] = (float)(rand() % 50) / 100;

    cudaMalloc(&d_kernel, sizeof(h_kernel));
    cudaMemcpy(d_kernel, h_kernel, sizeof(h_kernel), cudaMemcpyHostToDevice);

    const float alpha = 1, beta = 1;

    // Convolution forward
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);
    checkCUDNN(cudnnConvolutionForward(cudnn, &alpha, input_descriptor, d_input, kernel_descriptor, d_kernel, convolution_descriptor, convolution_algorithm, d_workspace, workspace_bytes, &beta, output_descriptor, d_output));
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    printf("Running time : %f\n", milliseconds / (3.0f * 1000.0f));

    float *h_output = (float *)malloc(image_bytes);
    cudaMemcpy(h_output, d_output, image_bytes, cudaMemcpyDeviceToHost);

    // Output results to file
    FILE *fp = fopen("result.txt", "w");
    for (int i = 0; i < OUT_WIDTH * OUT_HEIGHT; ++i)
        fprintf(fp, "%f ", h_output[i]);

    // Cleanup
    cudaFree(d_kernel);
    cudaFree(d_input);
    cudaFree(d_output);
    free(image);
    free(h_output);
    cudaFree(d_workspace);

    cudnnDestroyTensorDescriptor(input_descriptor);
    cudnnDestroyTensorDescriptor(output_descriptor);
    cudnnDestroyConvolutionDescriptor(convolution_descriptor);
    cudnnDestroyFilterDescriptor(kernel_descriptor);
    cudnnDestroy(cudnn);

    return 0;
}
