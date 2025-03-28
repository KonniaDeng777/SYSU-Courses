import cv2
import numpy as np
import matplotlib.pyplot as plt


def show_images(original, noisy, inverse, wiener):
    plt.figure(figsize=(20, 6))
    plt.subplot(141), plt.imshow(original, cmap='gray'), plt.title('Original Image')
    plt.subplot(142), plt.imshow(noisy, cmap='gray'), plt.title('Noisy Image')
    plt.subplot(143), plt.imshow(inverse, cmap='gray'), plt.title('Inverse_Restored Image')
    plt.subplot(144), plt.imshow(wiener, cmap='gray'), plt.title('Wiener_Restored Image')
    plt.show()


# 读取图像并转换为灰度图
img = cv2.imread('lena.png')
img = img.astype(np.uint8)
img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# 生成运动模糊核
kernel_size = 15  # 模糊核的大小
angle = 45  # 模糊核的角度
kernel_motion_blur = np.zeros((kernel_size, kernel_size))
kernel_motion_blur[int((kernel_size - 1) / 2), :] = np.ones(kernel_size)
kernel_motion_blur = cv2.warpAffine(kernel_motion_blur,
                                    cv2.getRotationMatrix2D((kernel_size / 2 - 0.5, kernel_size / 2 - 0.5), angle, 1.0),
                                    (kernel_size, kernel_size))
kernel_motion_blur = kernel_motion_blur / kernel_size

# 对图像进行卷积操作，得到运动模糊后的图像
img_blur = cv2.filter2D(img, -1, kernel_motion_blur)

# 生成高斯随机噪声
noise_mean = 0  # 噪声的均值
noise_var = 0.01  # 噪声的方差
noise = np.random.normal(noise_mean, noise_var ** 0.5, img.shape)

# 加到运动模糊后的图像上，得到最终退化后的图像
img_noisy = img_blur + noise


# 对退化后的图像进行傅里叶变换，并计算其幅度谱和相位谱
img_noisy_fft = np.fft.fft2(img_noisy)
img_noisy_fft_shift = np.fft.fftshift(img_noisy_fft)
img_noisy_magnitude = np.abs(img_noisy_fft_shift)
img_noisy_phase = np.angle(img_noisy_fft_shift)
img_noisy = np.clip(img_noisy, 0, 255).astype(np.uint8)
# 对运动模糊核进行傅里叶变换，并计算其幅度谱和相位谱
kernel_motion_blur_fft = np.fft.fft2(kernel_motion_blur, s=img.shape)
kernel_motion_blur_fft_shift = np.fft.fftshift(kernel_motion_blur_fft)
kernel_motion_blur_magnitude = np.abs(kernel_motion_blur_fft_shift)
kernel_motion_blur_phase = np.angle(kernel_motion_blur_fft_shift)

# 用逆滤波对退化后的图像进行复原，并计算其幅度谱和相位谱
img_inverse_fft_shift = img_noisy_fft_shift / kernel_motion_blur_fft_shift
img_inverse_magnitude = np.abs(img_inverse_fft_shift)
img_inverse_phase = np.angle(img_inverse_fft_shift)

# 用维纳滤波对退化后的图像进行复原，并计算其幅度谱和相位谱
K = 0.02  # 正则参数
img_wiener_fft_shift = kernel_motion_blur_fft_shift.conj() * img_noisy_fft_shift / (
            np.abs(kernel_motion_blur_fft_shift) ** 2 + K)
img_wiener_magnitude = np.abs(img_wiener_fft_shift)
img_wiener_phase = np.angle(img_wiener_fft_shift)

# 对复原后的图像进行逆傅里叶变换，并转换为灰度图
img_inverse_fft_ishift = np.fft.ifftshift(img_inverse_fft_shift)
img_inverse_restored = np.fft.ifft2(img_inverse_fft_ishift)
img_inverse_restored = np.real(img_inverse_restored)
img_inverse_restored = np.clip(img_inverse_restored, 0, 255).astype(np.uint8)

img_wiener_fft_ishift = np.fft.ifftshift(img_wiener_fft_shift)
img_wiener_restored = np.fft.ifft2(img_wiener_fft_ishift)
img_wiener_restored = np.real(img_wiener_restored)
img_wiener_restored = np.clip(img_wiener_restored, 0, 255).astype(np.uint8)

# 显示原始图像、退化后的图像和复原后的图像
show_images(img,img_noisy,img_inverse_restored,img_wiener_restored)
