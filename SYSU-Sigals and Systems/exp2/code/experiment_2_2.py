# 导入 需要的 library 库  
import numpy as np  # 科学计算
import matplotlib.pyplot as plt  # 画图
import scipy.signal as sg  # 导入 scipy 的 signal 库 命名为 sg

n1 = np.linspace(0, 5, 6)  # 时间序列[0 1 2 3 4 5]
x1 = [1, 2, 1, 1, 0, -3]  # 信号x[n]
fig, axs = plt.subplots(2, 2, figsize=(10, 10))  # 通过figsize调整图大小
plt.subplot(221)  # 绘制x[n]信号的子图
plt.stem(n1, x1, '-', use_line_collection=True)  # 绘制x[n]信号
plt.grid(True)  # 显示网格
_ = plt.title('x[n]')  # 信号x[n] title

n2 = np.linspace(0, 2, 3)  # 时间序列[0 1 2]
x2 = [1, -1, 1]  # 信号h[n]
plt.subplot(222)  # 绘制h[n]信号的子图
plt.stem(n2, x2, '-', use_line_collection=True)  # 绘制h[n]信号
plt.grid(True)  # 显示网格
_ = plt.xticks(np.arange(0, 3, step=1.0))  # 设置横坐标间隔
_ = plt.title('h[n]')  # 信号h[n] title

plt.subplot(212)  # 绘制卷积信号的子图
y = sg.convolve(x1, x2, 'full')  # 使用 scipy.signal 的卷积函数 convolve
n3 = np.linspace(0, 7, 8)
plt.stem(n3, y, '-', use_line_collection=True)  # 绘制卷积信号
plt.grid(True)  # 显示网格
_ = plt.title('Conv Sum y[n]')  # 卷积和信号y[n] title

plt.xlabel('Time index n')  # 时间轴
plt.subplots_adjust(top=1, wspace=0.2, hspace=0.2)  # 调整视图
plt.show()  # 显示图像
