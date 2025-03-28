import numpy as np
import scipy.signal as sg
import matplotlib.pyplot as plt
from scipy.integrate import simps  # 用于求积分

import warnings

warnings.filterwarnings("ignore")  # 去掉常规警告


def DTFT(n, xn, k=1, a=-2 * np.pi, b=2 * np.pi):
    ws = np.linspace(a, b, 1024)  # 频率取点
    Xw = np.zeros_like(ws, dtype=complex)
    for i in range(len(ws)):
        Xw[i] = np.sum(xn * np.exp(-1j * k * ws[i] * n))  # 利用公式计算DTFT
    return ws, Xw


def getY(w, Xw):
    Yw = np.zeros_like(w, dtype=complex)
    for i in range(len(w)):
        Yw[i] = Xw[i] * Xw[i]
    return Yw


# 定义离散时间双边指数衰减信号x[n] = e^(-2|n|)
n = np.arange(-10, 11)  # 时间序列范围
xn = np.exp(-2 * abs(n))
fs = 2 * np.pi  # 频域范围

# 根据卷积性质，计算频域响应Y(e^(jω))
w, Xw = DTFT(n, xn, 1, -fs, fs)
Yw = getY(w, Xw)

# 计算离散时间信号y[n]
yn = np.zeros_like(n, dtype=float)  # 时域信号值，这里一定要显示设置dtype=float，否则默认为int32
for i in range(len(n)):  # 用于计算离散信号x[n]
    yn[i] = simps(Yw * np.exp(1j * w * n[i]), w) / (
        w[-1] - w[0])  # 辛普森积分，积分默认区间为[-2π, 2π]，w[0]=-2π，w[-1]=2π

# 绘制离散时间信号x[n]的图像
fig, axs = plt.subplots(2, 1, figsize=(6, 8))  # 通过figsize调整图大小
plt.subplots_adjust(wspace=0, hspace=0.4)  # 通过hspace调整子图间距
plt.subplot(311)
plt.stem(n, xn)
plt.xlabel('n')
plt.ylabel('x[n]')
plt.title('Discrete-Time Signal x[n]')
plt.grid(True)

# 绘制频域响应|Y(e^(jω))|的图像
plt.subplot(312)
plt.plot(w, np.abs(Yw))
xticks = np.array([-2 * np.pi, -np.pi, 0, np.pi, 2 * np.pi])  # 用于控制横坐标的显示范围
xlabels = ['$-2\pi$', '$-\pi$', '0', '$\pi$', '$2\pi$']
plt.xticks(xticks, xlabels)
plt.xlabel('ω')
plt.ylabel('|Y(e^(jω))|')
plt.title('Magnitude Spectrum of Y(e^(jω))')
plt.grid(True)

# 绘制离散时间信号y[n]的图像
n_y = np.arange(-20, 21)  # 时间序列范围为-20到20，为了与卷积结果对齐
plt.subplot(313)
plt.stem(n, yn)
plt.xlabel('n')
plt.ylabel('y[n]')
plt.title('Discrete-Time Signal y[n]')
plt.grid(True)

# 调整子图间的间距
plt.subplots_adjust(hspace=0.5)

# 显示图像
plt.show()
