import cv2
import numpy as np
import matplotlib.pyplot as plt


def RGB(image):
    rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    # 计算每个通道的直方图
    hist_r, bins_r = np.histogram(rgb_image[:, :, 0].ravel(), 256, [0, 256])
    hist_g, bins_g = np.histogram(rgb_image[:, :, 1].ravel(), 256, [0, 256])
    hist_b, bins_b = np.histogram(rgb_image[:, :, 2].ravel(), 256, [0, 256])

    # 可视化直方图
    plt.figure(figsize=(8, 6))
    plt.title('RGB Histogram')
    plt.plot(bins_r[:-1], hist_r, color='r', label='Red')
    plt.plot(bins_g[:-1], hist_g, color='g', label='Green')
    plt.plot(bins_b[:-1], hist_b, color='b', label='Blue')
    plt.xlabel('Intensity')
    plt.ylabel('Count')
    plt.legend()
    plt.show()
