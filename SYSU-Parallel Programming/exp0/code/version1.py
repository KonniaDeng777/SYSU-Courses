import numpy as np
import time

def matrix_multiply_serial(A, B):
    m, n = A.shape
    n, k = B.shape
    C = np.zeros((m, k))

    for i in range(m):
        for j in range(k):
            for l in range(n):
                C[i][j] += A[i][l] * B[l][j]

    return C


def generate_random_matrix(rows, cols):
    return np.random.rand(rows, cols)

def calculate_peak_performance(m, n, k, execution_time):
    total_flops = 2 * m * n * k
    peak_performance = total_flops / execution_time
    return peak_performance


m = int(input("Enter values for m(between 512 and 2048):"))
n = int(input("Enter values for n(between 512 and 2048):"))
k = int(input("Enter values for k(between 512 and 2048):"))


A = generate_random_matrix(m, n)
B = generate_random_matrix(n, k)


start_time = time.time()
C = matrix_multiply_serial(A, B)
end_time = time.time()
execution_time = end_time - start_time

total_flops = 2 * m * n * k  
flops_per_second = total_flops / execution_time

peak_performance = calculate_peak_performance(m, n, k, execution_time)
actual_performance_percentage = (execution_time / peak_performance) * 100


print("Matrix A:")
print(A)
print("Matrix B:")
print(B)
print("Matrix C:")
print(C)
print("Running time: %f seconds" % execution_time)
print("FLOPS: %f " % flops_per_second)
print("peak performance: %f " % actual_performance_percentage)