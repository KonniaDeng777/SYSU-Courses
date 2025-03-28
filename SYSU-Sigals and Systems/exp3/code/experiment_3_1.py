# 导入 需要的 library 库
import numpy as np  # 科学计算
import matplotlib.pyplot as plt  # 画图
import scipy.signal as sg  # 导入 scipy 的 signal 库 命名为 sg
# import control as ctrl  # 导入 conctrl 库 命名为 ctrl
import sympy  # 符号运算
from scipy.integrate import quad  # 用于求解定积分

import warnings

warnings.filterwarnings("ignore")  # 去掉常规警告
from fontTools.ttLib import TTFont
import matplotlib.font_manager as fm

# # 读取字体文件
# font = TTFont('SimHei.ttf')
# # 将字体文件转换为 Matplotlib 可以识别的格式
# font.save('SimHei.xml')

# 将转换后的字体文件添加到 Matplotlib 字体列表中
fm.fontManager.addfont('SimHei.xml')
plt.rcParams['font.family'] = 'SimHei'

T = 4  # 基波周期
tao = 2  # 宽度，即有效区间[-T1,T1],tao=2*T1
omega = 2 * np.pi / T  # 基波频率
A = 1  # 幅度
N = 10  # 系数显示至N次谐波
## 傅里叶级数频谱图
Xn = np.zeros(2 * N + 1)  # 傅里叶级数指数形式的幅值
nn = np.arange(-N, N + 1)  # 傅里叶级数项数-N~N
for n in range(-N, N + 1):
    Func = lambda t: A * np.exp(-1j * n * omega * t) / T
    Xn[n + N] = quad(Func, -tao / 2, tao / 2)[0]
plt.figure()
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号
plt.stem(nn, Xn, use_line_collection=True)  # 绘制频谱图
_ = plt.title("周期矩形信号频谱图", {'size': 14})
_ = plt.xticks(np.arange(-N, N + 1, step=T))  # 横坐标间隔
plt.grid(True)  # 显示网格
plt.show()  # 显示图像
