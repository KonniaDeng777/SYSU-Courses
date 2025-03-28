# 导入numpy和scipy库
import numpy as np
import cv2
from PIL import ImageEnhance, Image
from numpy.fft import fft2, ifft2
from scipy.signal import convolve2d
import matplotlib.pyplot as plt


def show_images(noisy, restored):
    plt.figure(figsize=(12, 6))
    plt.subplot(121), plt.imshow(noisy, cmap='gray'), plt.title('Noisy Image')
    plt.subplot(122), plt.imshow(restored, cmap='gray'), plt.title('Restored Image')
    plt.show()


def motion_blur(img, size, angle):
    # 创建运动模糊滤波器
    blur_filter = np.ones((size, size))
    blur_filter[int((size - 1) / 2), :] = 0
    blur_filter = cv2.getRotationMatrix2D((size / 2 - 1, size / 2 - 1), angle, 1)
    M = np.eye(size)
    M[:2, :3] = blur_filter
    blur_filter = cv2.warpAffine(M, blur_filter, (size, size))
    blur_filter = blur_filter / np.sum(blur_filter)
    # 应用滤波器
    return cv2.filter2D(img, -1, blur_filter)


def add_gaussian_noise(img, mean=0, sigma=10):
    # 添加高斯噪声
    noise = np.zeros_like(img)
    cv2.randn(noise, mean, sigma)
    noisy_img = cv2.add(img, noise, dtype=cv2.CV_8UC3)
    return noisy_img


# 定义一个高斯核函数
def gauss_kernel(size, sizey=None):
    size = int(size)
    if not sizey:
        sizey = size
    else:
        sizey = int(sizey)
    x, y = np.mgrid[-size:size + 1, -sizey:sizey + 1]
    g = np.exp(-(x ** 2 / float(size) + y ** 2 / float(sizey)))
    return g / g.sum()


# 定义一个维纳滤波函数
def wiener_filter(img, kernel, K):
    kernel /= np.sum(kernel)
    dummy = np.copy(img)
    dummy = fft2(dummy)
    kernel = fft2(kernel, s=img.shape)
    kernel = np.conj(kernel) / (np.abs(kernel) ** 2 + K)
    dummy = dummy * kernel
    dummy = np.abs(ifft2(dummy))
    return dummy


# 读取noisy.png文件
img = cv2.imread("noisy.png", 0)
cv2.imwrite("noisy.png", img)

# 使用高斯核模拟退化过程
kernel = gauss_kernel(3)

# 使用维纳滤波恢复图像
filtered_img = wiener_filter(img, kernel, K=0.001)
# 保存恢复后的图像
cv2.imwrite("restored.png", filtered_img)
# 创建ImageEnhance对象

f_img = Image.open("restored.png")
# filtered_img=Image.fromarray(filtered_img)
enhancer = ImageEnhance.Brightness(f_img)  # 亮度增强
enhancer = ImageEnhance.Contrast(f_img)  # 对比度增强
# enhancer = ImageEnhance.Color(f_img)  # 色彩增强
enhancer = ImageEnhance.Sharpness(f_img)  # 锐度增强

# 调整增强因子
factor = 1.2  # 可以根据需要调整

# 得到增强后的图像
img_enhanced = enhancer.enhance(factor)

show_images(img, img_enhanced)
