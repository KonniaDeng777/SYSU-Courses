import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as sg

import warnings

warnings.filterwarnings("ignore")


def continuous_step_signal():
    t = np.linspace(-1, 1, 500, endpoint=False)
    plt.plot(t, np.array(t > 0, dtype=np.int))
    plt.grid()
    _ = plt.ylim(-0.5, 1.5)  # 限制 y 轴范围
    plt.show()


continuous_step_signal()


def continuous_exp_signal():
    t = np.linspace(0, 5, 5000, endpoint=False)
    plt.plot(t, 2 * np.exp(-1 * t))
    plt.grid()
    plt.show()


continuous_exp_signal()


def continuous_sin_signal():
    t = np.linspace(0, 1, 500, endpoint=False)
    A = 1  # 振幅
    phi = np.pi / 6  # 初相位
    fre = 10  # 频率
    plt.plot(t, A * np.sin(t * 2 * np.pi * fre + phi))
    plt.grid()
    plt.show()


continuous_sin_signal()


def rect_wave(t, c, c0):  # 起点为c0，宽度为c的矩形波
    if t >= (c + c0):
        r = 0.0
    elif t < c0:
        r = 0.0
    else:
        r = 1
    return r


def continuous_rectangular_pulse_signal():
    x = np.linspace(-4, 8, 1000)
    y = np.array([rect_wave(t, 4.0, -2.0) for t in x])
    _ = plt.ylim(-0.2, 1.2)
    plt.plot(x, y)
    plt.grid()
    plt.show()


continuous_rectangular_pulse_signal()


def continuous_rectangle_signal():
    t = np.linspace(0, 1, 500, endpoint=False)
    '''
    sg.square(w0*t,duty=0.5)
    频率为w0，占空比默认0.5
    '''
    # 产生一个振幅为1，频率为5Hz，占空比为50%的周期方波
    plt.plot(t, sg.square(2 * np.pi * 5 * t))
    plt.ylim(-2, 2)  # y的范围
    plt.grid()
    plt.show()


continuous_rectangle_signal()


def continuous_sample_signal():
    # 使用sinc(x)计算抽样信号，sinc(x)=sin(pi*x)/(pi*x)
    t = np.linspace(-10, 10, 500, endpoint=False)
    plt.plot(t, np.sin(t))
    plt.grid()
    plt.show()


continuous_sample_signal()


def unit_pulse():
    # 定义时间序列
    n = np.linspace(-4, 21, 25, endpoint=False)
    # 产生单位脉冲序列
    # np.zeros((1,n))产生1行n列由0组成的矩阵
    # np.ones((1,n))产生1行n列由1组成的矩阵
    x = np.append(np.zeros((1, 7)), np.ones((1, 1)))
    x = np.append(x, np.zeros((1, 17)))
    plt.stem(n, x, use_line_collection=True)
    plt.grid()
    plt.show()


unit_pulse()


def unit_step():
    n = np.linspace(-4, 21, 25, endpoint=False)
    x = np.append(np.zeros((1, 7)), np.ones((1, 18)))
    plt.stem(n, x, use_line_collection=True)
    plt.grid()
    plt.show()


unit_step()


def exp():
    n = np.linspace(-4, 21, 25, endpoint=False)
    x = 0.3 * np.power(1 / 2, n)  # 0.3*0.5^n
    plt.stem(n, x, use_line_collection=True)
    plt.grid()
    plt.show()


exp()


def sin():
    n = np.linspace(-50, 51, 101, endpoint=False)
    omega = np.pi / 10  # 频率
    x = 0.3 * np.sin(omega * n + np.pi / 5)  # 产生正弦序列
    plt.stem(n, x, use_line_collection=True)
    plt.grid()
    plt.show()


sin()


def discrete_rectangle_wave():
    # 振幅为1，基频为rad，占空比为50%
    n = np.linspace(-10, 11, 21, endpoint=False)
    rad = np.pi / 4
    plt.stem(n, sg.square(rad * n, 0.5))
    plt.xticks(np.arange(-10, 10, step=5.0))  # 横坐标间距
    plt.grid()
    plt.show()


discrete_rectangle_wave()
