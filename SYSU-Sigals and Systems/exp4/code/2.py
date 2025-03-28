import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as sg

# 定义差分方程的系数
a = 0.2


# 求解差分方程的频率响应
def freq_response(a):
    num = [1]
    den = [1, -a]
    ws = np.linspace(-2 * np.pi, 2 * np.pi, 1024)
    w, H = sg.freqz(num, den, ws)
    return w, H


# 求解系统的单位脉冲响应
def impulse_response(a, num_points):
    num = [1]
    den = [1, a]
    imp_resp = sg.impulse([num, den], N=num_points)
    return imp_resp[0], imp_resp[1]


# 求解频率响应
w, H = freq_response(a)

# 绘制幅频特性曲线和相频特性曲线
fig, axs = plt.subplots(2, 1, figsize=(6, 6))
plt.subplots_adjust(hspace=0.4)
plt.subplot(211)
plt.plot(w, np.abs(H))
xticks = np.array([-2 * np.pi, -np.pi, 0, np.pi, 2 * np.pi])  # 用于控制横坐标的显示范围
xlabels = ['$-2\pi$', '$-\pi$', '0', '$\pi$', '$2\pi$']
plt.xticks(xticks, xlabels)
plt.xlabel('$\omega$')
plt.title('$|H(e^{jw})|$')
plt.grid()  # 显示网格

plt.subplot(212)
plt.plot(w, np.angle(H))
xticks = np.array([-2 * np.pi, -np.pi, 0, np.pi, 2 * np.pi])  # 用于控制横坐标的显示范围
xlabels = ['$-2\pi$', '$-\pi$', '0', '$\pi$', '$2\pi$']
plt.xticks(xticks, xlabels)
plt.xlabel('$\omega$')
plt.title('$\measuredangle X(e^{jw})$')  # ∡
plt.grid()  # 显示网格
plt.show()  # 显示图像

# 求解单位脉冲响应
num_points = 50  # 单位脉冲响应的采样点数
n, impulse = impulse_response(a, num_points)

# 绘制单位脉冲响应图形
plt.stem(n, impulse)
plt.xlabel('n')
plt.ylabel('h[n]')
plt.title('Impulse Response')
plt.grid()
plt.show()
