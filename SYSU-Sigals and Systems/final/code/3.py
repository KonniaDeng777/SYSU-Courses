import cv2
import numpy as np
import random
import matplotlib.pyplot as plt


def show_images(original, filtered):
    plt.figure(figsize=(12, 6))
    plt.subplot(121), plt.imshow(cv2.cvtColor(original, cv2.COLOR_BGR2RGB)), plt.title('Original Image')
    plt.subplot(122), plt.imshow(cv2.cvtColor(filtered, cv2.COLOR_BGR2RGB)), plt.title('Noisy Image')
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


# 读入图像
img = cv2.imread('lena.png')

# 进行运动模糊和高斯噪声退化
motion_blurred = motion_blur(img, 30, 45)
noisy_motion_blurred = add_gaussian_noise(motion_blurred)

# 显示结果
show_images(img, noisy_motion_blurred)

