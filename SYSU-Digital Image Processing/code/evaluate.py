import numpy as np
import skimage
from scipy.ndimage import gaussian_filter
from skimage.metrics import peak_signal_noise_ratio, mean_squared_error
from scipy import ndimage
from scipy.signal import convolve2d

# 读取原始图像和重建图像
original_image = skimage.io.imread('0.jpg')
reconstructed_image = skimage.io.imread('results/0.jpg')

original_image_1 = original_image.astype(np.float64) / 255.0
reconstructed_image_1 = reconstructed_image.astype(np.float64) / 255.0

# 计算PSNR
psnr = peak_signal_noise_ratio(original_image_1, reconstructed_image_1)

# 计算MSE
mse = mean_squared_error(original_image_1, reconstructed_image_1)

# 打印结果
print("PSNR:", psnr)
print("MSE:", mse)


def ssim(image1, image2):
    # 使用高斯滤波器计算图像的均值、标准差和协方差
    mu1 = gaussian_filter(image1, sigma=1.5)
    mu2 = gaussian_filter(image2, sigma=1.5)
    sigma1_sq = gaussian_filter(image1 ** 2, sigma=1.5) - mu1 ** 2
    sigma2_sq = gaussian_filter(image2 ** 2, sigma=1.5) - mu2 ** 2
    sigma12 = gaussian_filter(image1 * image2, sigma=1.5) - mu1 * mu2

    # 定义常数
    K1 = 0.01
    K2 = 0.03
    L = 1.0

    # 计算SSIM
    numerator = (2 * mu1 * mu2 + K1) * (2 * sigma12 + K2)
    denominator = (mu1 ** 2 + mu2 ** 2 + K1) * (sigma1_sq + sigma2_sq + K2)
    ssim_map = numerator / denominator
    ssim_score = np.mean(ssim_map)

    return ssim_score


# 将图像转换为灰度图像
original_image_2 = skimage.color.rgb2gray(original_image)
reconstructed_image_2 = skimage.color.rgb2gray(reconstructed_image)

# 计算SSIM
ssim_score = ssim(original_image_2, reconstructed_image_2)

# 打印结果
print("SSIM:", ssim_score)