# 导入 需要的 library 库
import math
import numpy as np  # 科学计算
import matplotlib.pyplot as plt  # 画图
import scipy.signal as sg  # 导入 scipy 的 signal 库 命名为 sg

import warnings

warnings.filterwarnings("ignore")  # 去掉常规警告


def getX(a, k=1, m=-2 * np.pi, n=2 * np.pi):
    ws = np.linspace(m, n, 1024)  # 频率取点
    Xw = lambda w: (1 - np.power(a, 2)) / (1 - 2 * a * np.cos(k * w) + np.
                                           power(a, 2))
    return ws, Xw


n = np.arange(64)  # 定义序号
a = np.exp(-2)
xn = np.power(a, abs(n))
fs = 2 * np.pi  # 频域范围
w, Xw = getX(a, 1, -fs, fs)

fig, axs = plt.subplots(2, 1, figsize=(6, 6))  # 通过figsize调整图大小
plt.subplots_adjust(wspace=0, hspace=0.4)  # 通过hspace调整子图间距
plt.subplot(211)  # 绘制x[n]信号的子图
plt.plot(w, np.abs(Xw(w)))  # 绘制X(e^(jw))的幅频曲线
xticks = np.array([-2 * np.pi, -np.pi, 0, np.pi, 2 * np.pi])  # 用于控制横坐标的显示范围
xlabels = ['$-2\pi$', '$-\pi$', '0', '$\pi$', '$2\pi$']
plt.xticks(xticks, xlabels)
plt.xlabel('$\omega$')
plt.title('$|X(e^{jw})|$')
plt.grid()  # 显示网格

plt.subplot(212)  # 绘制X(e^(jw))的相频曲线
plt.plot(w, np.angle(Xw(w)))  # 绘制随时间衰减的单边指数信号DTFT相频图
xticks = np.array([-2 * np.pi, -np.pi, 0, np.pi, 2 * np.pi])  # 用于控制横坐标的显示范围
xlabels = ['$-2\pi$', '$-\pi$', '0', '$\pi$', '$2\pi$']
plt.xticks(xticks, xlabels)
plt.xlabel('$\omega$')
plt.title('$\measuredangle X(e^{jw})$')  # ∡
plt.grid()  # 显示网格
plt.show()  # 显示图像

kw, kXw = getX(a, 3, -fs, fs)

fig, axs = plt.subplots(2, 1, figsize=(6, 6))  # 通过figsize调整图大小
plt.subplots_adjust(wspace=0, hspace=0.4)  # 通过hspace调整子图间距
plt.subplot(211)  # 绘制x[n]信号的子图
plt.plot(kw, np.abs(kXw(w)))  # 绘制X(e^(jw))的幅频曲线
xticks = np.array([-2 * np.pi, -np.pi, 0, np.pi, 2 * np.pi])  # 用于控制横坐标的显示范围
xlabels = ['$-2\pi$', '$-\pi$', '0', '$\pi$', '$2\pi$']
plt.xticks(xticks, xlabels)
plt.xlabel('$\omega$')
plt.title('$|Xk(e^{jw})|$')
plt.grid()  # 显示网格

plt.subplot(212)  # 绘制x[n]傅里叶变换的相频曲线
plt.plot(kw, np.angle(kXw(w)))  # 绘制随时间衰减的单边指数信号DTFT相频图
# plt.plot(w, np.angle(Xw), w, np.angle(1/(1-a*np.exp(-1j*w)))) # 第二个图为解析解形式
xticks = np.array([-2 * np.pi, -np.pi, 0, np.pi, 2 * np.pi])  # 用于控制横坐标的显示范围
xlabels = ['$-2\pi$', '$-\pi$', '0', '$\pi$', '$2\pi$']
plt.xticks(xticks, xlabels)
plt.xlabel('$\omega$')
plt.title('$\measuredangle Xk(e^{jw})$')  # ∡
plt.grid()  # 显示网格
plt.show()  # 显示图像