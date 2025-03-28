import cv2
import numpy as np
import matplotlib.pyplot as plt

#针对水下色偏,基于白平衡的颜色校正问题
def white_balance(image):
    balanced_image = np.zeros_like(image)  # 创建一个与输入图像相同大小的空白图像

    # 对每个颜色通道进行白平衡处理
    for i in range(3):
        # 1. 排序
        sorted_gray_values = np.sort(image[:, :, i].flatten())

        # 2. 选取部分区域像素
        total_pixels = image.shape[0] * image.shape[1]
        low_pixels = int(total_pixels * 0.1)  # 选取比例低亮度的像素个数
        high_pixels = int(total_pixels * 0.1)  # 选取比例高亮度的像素个数

        Imin = sorted_gray_values[low_pixels]  # 最低亮度像素的灰度值
        Imax = sorted_gray_values[-high_pixels]  # 最高亮度像素的灰度值

        # 3. 像素填充
        balanced_channel = np.where(image[:, :, i] <= Imin, 0, image[:, :, i])  # 小于等于 Imin 的像素设为 0
        balanced_channel = np.where(balanced_channel >= Imax, 255, balanced_channel)  # 大于等于 Imax 的像素设为 255

        # 4. 像素映射
        balanced_channel = ((balanced_channel - Imin) * 255 / (Imax - Imin)).clip(0, 255)

        # 将处理后的通道赋值给空白图像的对应通道
        balanced_image[:, :, i] = balanced_channel

    return balanced_image



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

# 读取图像
image = cv2.imread('111/2129.jpg')
RGB(image)

# 进行白平衡处理
balanced_image = white_balance(image.astype(np.float32) / 255.0).astype(np.uint8)
RGB(balanced_image)

# 显示处理前后的图像
cv2.imshow('Original Image', image)
cv2.imshow('Balanced Image', balanced_image)
cv2.waitKey(0)
cv2.destroyAllWindows()