import numpy as np
import matplotlib.pyplot as plt


# 定义给定的函数
def x(t):
    return u(t) - u(t - 2) + u(t - 0.5) - u(t - 1.5)


# 定义单位阶跃函数u(t)
def u(t):
    return (t >= 0).astype(float)


# 绘制x(-2t)的波形
def draw_1():
    t = np.linspace(-3, 3, 1000)
    x1 = x(-2 * t)
    plt.figure()
    plt.plot(t, x1)
    plt.title('x(-2t)')
    plt.xlabel('t')
    plt.ylabel('x(-2t)')
    plt.grid(True)
    plt.show()


def draw_2():
    # 绘制x(t/2+1)的波形
    t = np.linspace(-3, 3, 1000)
    x2 = x(t / 2 + 1)
    plt.figure()
    plt.plot(t, x2)
    plt.title('x(t/2 + 1)')
    plt.xlabel('t')
    plt.ylabel('x(t/2 + 1)')
    plt.grid(True)
    plt.show()


def draw_3():
    # 绘制5x(t)的波形
    t = np.linspace(-3, 3, 1000)
    x3 = 5 * x(t)
    plt.figure()
    plt.plot(t, x3)
    plt.title('5x(t)')
    plt.xlabel('t')
    plt.ylabel('5x(t)')
    plt.grid(True)
    plt.show()


draw_1()
draw_2()
draw_3()




