# 导入 需要的 library 库  
import numpy as np  # 科学计算
import matplotlib.pyplot as plt  # 画图


# noinspection PyPep8Naming
def triangle_wave(x, width, skew):  # 幅度为hc=1，宽度为width,斜度为skew的三角波，skew范围[-1, 1]，当skew=0，产生对称的三角波信号
    # 产生幅度为hc，宽度为width，且以0为中心左右各展开width/2大小，斜度为skew的三角波。
    if not (-1 <= skew <= 1):
        raise Exception("skew value ERROR!")   # skew范围不对，抛出异常
    hc = 1  # 三角波默认最大幅度，可以通过外部直接乘一个幅度值改变，该点横坐标通过下式计算
    xPoint = width / 2 * skew  # 三角波信号拐点横坐标，即上升沿和下降沿的横坐标

    if (x >= width / 2) or (x <= -width / 2):  # 宽度之外的值为0
        r = 0.0
    elif x > xPoint:  # 下降沿的函数
        r = -(x - xPoint) / (width / 2 - xPoint) + hc
    else:  # 上升沿的函数
        r = (x - xPoint) / (width / 2 + xPoint) + hc
    return r


x = np.linspace(-3, 3, 1000)  # 定义时间序列
y = np.array([triangle_wave(t, 4.0, 0.5) for t in x])  # x(t)信号
y2 = np.array([triangle_wave(2 * t, 4.0, 0.5) for t in x])  # x(2t)信号
y3 = np.array([triangle_wave(1 - 2 * t, 4.0, 0.5) for t in x])  # x(1-2t)信号
fig, axs = plt.subplots(3, 1, figsize=(10, 10))  # 通过figsize调整图大小
plt.subplots_adjust(wspace=0, hspace=0.4)  # 通过hspace调整子图间距
plt.subplot(311)  # 绘制x(t)信号的子图
plt.grid()  # 显示网格
plt.title('x(t)')  # x(t)信号的子图title
plt.plot(x, y)  # 绘制x(t)信号
plt.subplot(312)  # 绘制x(t)信号的子图
plt.grid()  # 显示网格
plt.title('x(2t)')  # x(2t)信号的子图title
plt.plot(x, y2)  # 绘制x(2t)信号
plt.subplot(313)  # 绘制x(1-2t)信号的子图
plt.grid()  # 显示网格
plt.title('x(1-2t)')  # x(1-2t)信号的子图title
plt.plot(x, y3)  # 绘制x(1-2t)信号
plt.show()  # 显示图像
