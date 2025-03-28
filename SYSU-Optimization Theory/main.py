import os

import numpy as np
import matplotlib.pyplot as plt

# 设置参数和系统大小
num_nodes = 10
num_features = 200
num_measurements = 5
sparsity = 5
lambda_val = 10
lambda_str = "10"
num_iterations = 1000

# 生成稀疏向量x的真值
np.random.seed(0)
x_true = np.zeros(num_features)
nonzero_indices = np.random.choice(num_features, sparsity, replace=False)
x_true[nonzero_indices] = np.random.normal(0, 1, sparsity)

# 生成测量矩阵A和测量噪声e
A = np.random.normal(0, 1, (num_nodes, num_measurements, num_features))
e = np.random.normal(0, 0.1, (num_nodes, num_measurements))

# 生成测量值b
b = np.zeros((num_nodes, num_measurements))
for i in range(num_nodes):
    b[i] = np.dot(A[i], x_true) + e[i]


# 邻近点梯度法
def proximal_gradient_method():
    x = np.zeros(num_features)
    x_prev = np.zeros(num_features)
    distance_to_true = []
    distance_to_optimal = []

    for _ in range(num_iterations):
        gradient = np.zeros(num_features)
        for i in range(num_nodes):
            gradient += np.dot(A[i].T, np.dot(A[i], x) - b[i])
        gradient += lambda_val * np.sign(x)
        x_prev = x.copy()
        x = x_prev - 0.001 * gradient  # 步长为0.001

        distance_to_true.append(np.linalg.norm(x - x_true))
        distance_to_optimal.append(np.linalg.norm(x - x_prev))

    return x, distance_to_true, distance_to_optimal


# 交替方向乘子法
def alternating_direction_method():
    # x = np.zeros(num_features)
    # z = np.zeros(num_features)
    # u = np.zeros(num_features)
    # 使用正态分布初始化 x、z 和 u
    x = np.random.randn(num_features)
    z = np.random.randn(num_features)
    u = np.random.randn(num_features)

    distance_to_true = []
    distance_to_optimal = []

    for _ in range(num_iterations):
        x = np.linalg.solve(sum([np.dot(A[i].T, A[i]) for i in range(num_nodes)]) + lambda_val * np.eye(num_features),
                            sum([np.dot(A[i].T, b[i]) for i in range(num_nodes)]) + lambda_val * (z - u))
        z = np.maximum(0, x + u)
        u = u + x - z

        distance_to_true.append(np.linalg.norm(x - x_true))
        distance_to_optimal.append(np.linalg.norm(x - z))

    return x, distance_to_true, distance_to_optimal


# 次梯度法
def subgradient_method():
    x = np.zeros(num_features)
    distance_to_true = []
    distance_to_optimal = []

    for _ in range(num_iterations):
        subgradient = np.zeros(num_features)
        for i in range(num_nodes):
            subgradient += np.dot(A[i].T, np.dot(A[i], x) - b[i])
        subgradient += lambda_val * np.sign(x)

        step_size = 0.01 / np.sqrt(_ + 1)  # 步长为0.01 / sqrt(t+1)
        x = x - step_size * subgradient
        x = np.maximum(0, x)  # 非负投影

        distance_to_true.append(np.linalg.norm(x - x_true))
        distance_to_optimal.append(np.linalg.norm(subgradient))

    return x, distance_to_true, distance_to_optimal


# 运行算法并绘制结果
def run_algorithm(algorithm_name, algorithm_func, lambda_val):
    x, distance_to_true, distance_to_optimal = algorithm_func()
    iterations = range(1, num_iterations + 1)

    plt.plot(iterations, distance_to_true, label="Distance to True")
    plt.plot(iterations, distance_to_optimal, label="Distance to Optimal")
    plt.xlabel("Iterations")
    plt.ylabel("Distance")
    plt.title(f"{algorithm_name} Algorithm")
    plt.legend()
    plt.savefig('results/{}/{}'.format(lambda_val, algorithm_name))
    plt.show()


result_folder = "results"
os.makedirs(os.path.join(result_folder, lambda_str), exist_ok=True)
# 邻近点梯度法
run_algorithm("Proximal Gradient", proximal_gradient_method, lambda_val)

# 交替方向乘子法
run_algorithm("Alternating Direction Method", alternating_direction_method, lambda_val)

# 次梯度法
run_algorithm("Subgradient Method", subgradient_method, lambda_val)
