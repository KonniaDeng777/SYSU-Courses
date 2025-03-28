import cv2
import numpy as np
import matplotlib.pyplot as plt


def show_images(original, low, high, band):
    plt.figure(figsize=(20, 6))
    plt.subplot(141), plt.imshow(cv2.cvtColor(original, cv2.COLOR_BGR2RGB)), plt.title('Original Image')
    plt.subplot(142), plt.imshow(cv2.cvtColor(low, cv2.COLOR_BGR2RGB)), plt.title('Lowpass Image')
    plt.subplot(143), plt.imshow(cv2.cvtColor(high, cv2.COLOR_BGR2RGB)), plt.title('Highpass Image')
    plt.subplot(144), plt.imshow(cv2.cvtColor(band, cv2.COLOR_BGR2RGB)), plt.title('Bandpass Image')
    plt.show()


def butterworth_filter(img, filter_type, radius, order):
    # 转换为灰度图像
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img_shape = gray.shape

    # 傅里叶变换
    f_gray = np.fft.fft2(gray)
    f_gray_shift = np.fft.fftshift(f_gray)

    # 创建空的滤波器
    filter_mask = np.zeros(img_shape)

    # 计算滤波器中心
    center_x, center_y = img_shape[1] // 2, img_shape[0] // 2

    for x in range(img_shape[1]):
        for y in range(img_shape[0]):
            distance = np.sqrt((x - center_x) ** 2 + (y - center_y) ** 2)

            if filter_type == 'lowpass':
                filter_mask[y, x] = 1 / (1 + (distance / radius) ** (2 * order))
            elif filter_type == 'highpass':
                filter_mask[y, x] = 1 / (1 + (radius / distance) ** (2 * order))
            elif filter_type == 'bandpass':
                low_radius, high_radius = radius
                low_mask = 1 / (1 + (distance / low_radius) ** (2 * order))
                high_mask = 1 / (1 + (high_radius / distance) ** (2 * order))
                filter_mask[y, x] = low_mask * high_mask

    # 应用滤波器
    filtered_shift = f_gray_shift * filter_mask
    filtered = np.fft.ifftshift(filtered_shift)
    filtered_img = np.abs(np.fft.ifft2(filtered))

    # 转换为8位整数
    filtered_img = np.uint8(filtered_img)

    return filtered_img


img = cv2.imread('lena.png')
# 巴特沃斯低通滤波
lowpass_butter = butterworth_filter(img, 'lowpass', 30, 2)

# 巴特沃斯高通滤波
highpass_butter = butterworth_filter(img, 'highpass', 30, 2)

# 巴特沃斯带通滤波
bandpass_butter = butterworth_filter(img, 'bandpass', [30, 80], 2)

show_images(img, lowpass_butter,highpass_butter,bandpass_butter)
