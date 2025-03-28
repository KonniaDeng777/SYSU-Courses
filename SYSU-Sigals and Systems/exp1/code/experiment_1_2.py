# 导入 需要的 library 库  
import numpy as np  # 科学计算
import matplotlib.pyplot as plt  # 画图

A = 2  # 信号幅度
a = -0.5  # 指数信号底数
n = np.linspace(1, 10, 10)  # 离散时间序列
xn = A * np.power(a, n)  # 计算x(n)信号
fig, axs = plt.subplots(2, 1, figsize=(10, 5))  # 通过figsize调整图大小
plt.subplots_adjust(wspace=0, hspace=0.4)  # 通过hspace调整子图间距
plt.subplot(211)  # 绘制x(n)信号的子图
plt.stem(n, xn, use_line_collection=True)  # 绘制x(n)信号
plt.grid()  # 显示网格
_ = plt.title('x[n]')  # x[n]信号title
xn2 = A * np.power(a, 0.5 * n)  # 计算x(0.5n)信号
plt.subplot(212)  # 绘制x(0.5n)信号的子图
plt.stem(n, xn2, use_line_collection=True)  # 绘制x(0.5n)信号
plt.grid()  # 显示网格
_ = plt.title('x[0.5n]')  # x[0.5n]信号title
plt.show()  # 显示图像
