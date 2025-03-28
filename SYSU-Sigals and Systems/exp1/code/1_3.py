import numpy as np
import matplotlib.pyplot as plt


# 定义信号x[n]
def x(n):
    if n < -2:
        return -1
    elif -2 <= n <= 1:
        return n
    else:
        return 1 / n


# 绘制x[-n]
n = np.arange(-20, 21)
xn = [x(-i) for i in n]
plt.stem(n, xn)
plt.title('x[-n]')
plt.xlabel('n')
plt.ylabel('x[-n]')
plt.grid()
plt.show()

# 绘制x[2n+2]
n = np.arange(-20, 21)
xn = [x(2 * i + 2) for i in n]
plt.stem(n, xn)
plt.title('x[2n+2]')
plt.xlabel('n')
plt.ylabel('x[2n+2]')
plt.grid()
plt.show()

# 绘制x[n/2]
n = np.arange(-20, 21)
xn = [x(0.5 * i) for i in n]
even_n = n[::2]  # 取出偶数点的n值
even_xn = [xn[i] for i in range(len(xn)) if i % 2 == 0]  # 取出偶数点的xn值
plt.stem(even_n, even_xn)
plt.title('x[n/2]')
plt.xlabel('n')
plt.ylabel('x[n/2]')
plt.grid()
plt.show()

