# 导入 需要的 library 库  
import numpy as np  # 科学计算
import matplotlib.pyplot as plt  # 画图
import scipy.signal as sg  # 导入 scipy 的 signal 库 命名为 sg

fs = 100  # 采样频率，注意和时间轴右端点结合使用，用于控制右端点的范围
t1 = np.array([t / fs for t in range(-200, 211)])  # t in [-2.0, 2.1] # 时间序列，注意右端点控制在2.1范围内，即2.1=211/fs
x1 = np.array([1 if t >= 0 else 0 for t in t1])  # 定义x1(t)阶跃信号
t2 = np.array([t / fs for t in range(-200, 211)])  # t in [-2.0, 2.1] # 时间序列，注意右端点控制在2.1范围内，即2.1=211/fs
x2 = np.array([np.exp(-3 * t) if t >= 0 else 0 for t in t1])  # 定义x2(t)信号
y1 = sg.convolve(x1, x2) / fs  # 卷积
n = len(y1)  # 卷积结果采样点数量
tt = np.linspace(-400, 421, n) / fs  # 定义新序列时间范围，卷积结果时间轴，卷积左端点=x1左端点+x2左端点，卷积右端点=x1右端点+x2右端点-1，
fig, axs = plt.subplots(2, 2, figsize=(10, 10))  # 通过figsize调整图大小
plt.subplots_adjust(wspace=0.2, hspace=0.2)  # 通过wspace和hspace调整子图间距
plt.subplot(221)  # 绘制x1(t)信号的子图
plt.plot(t1, x1)  # 绘制x1(t)信号
plt.grid()  # 显示网格
_ = plt.title('x1(t)')  # x1(t)信号title
plt.subplot(222)  # 绘制x2(t)信号的子图
plt.plot(t2, x2)  # 绘制x2(t)信号
plt.grid()  # 显示网格
_ = plt.title('x2(t)')  # x2(t)信号title
plt.subplot(212)  # 绘制卷积信号的子图
plt.plot(tt, y1)  # 绘制卷积信号
plt.grid()  # 显示网格
_ = plt.title('conv(x1,x2)')  # 卷积信号title
plt.show()  # 显示图像
