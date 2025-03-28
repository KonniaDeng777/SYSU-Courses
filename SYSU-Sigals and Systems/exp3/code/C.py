import numpy as np
from scipy.integrate import odeint
import matplotlib.pyplot as plt


def input_signal(t):
    if t > -8 and t <= 12:
        return input_signal(t - 4)
    elif t >= -12 and t <= -10:
        return 2 * t + 22
    elif t > -10 and t <= -8:
        return -2 * t - 18
    else:
        return 0


def triangle_wave(t, T):
    t = t % T
    if t < T / 2:
        return 4 * t / T - 1
    else:
        return 3 - 4 * t / T


def system(y, t):
    dydt = -2 * y + input_signal(t)
    return dydt


def solve(T):
    t = np.linspace(-12, 12, 4000)
    y0 = 0
    y = odeint(system, y0, t)
    return t, y[:, 0]


t, x = np.linspace(-12, 12, 4000), np.zeros(4000)
for i in range(4000):
    x[i] = input_signal(t[i])

t, y = solve(6)

plt.plot(t, x, label='Input')
plt.plot(t, y, label='Output')
plt.legend()
plt.grid()
plt.show()
