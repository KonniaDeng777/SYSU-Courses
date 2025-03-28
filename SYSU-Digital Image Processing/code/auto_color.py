import cv2
import numpy as np
from RGB import RGB

# 自动色阶
def calculate_limits(image, factor):
    # 分别统计图像的蓝、绿、红三个通道的直方图
    hist_blue = cv2.calcHist([image], [0], None, [256], [0, 256])
    hist_green = cv2.calcHist([image], [1], None, [256], [0, 256])
    hist_red = cv2.calcHist([image], [2], None, [256], [0, 256])

    # 分别计算各通道的上下限值
    min_blue = np.argmax(hist_blue > factor * hist_blue.max())
    max_blue = 255 - np.argmax(np.flip(hist_blue) > factor * np.flip(hist_blue).max())

    min_green = np.argmax(hist_green > factor * hist_green.max())
    max_green = 255 - np.argmax(np.flip(hist_green) > factor * np.flip(hist_green).max())

    min_red = np.argmax(hist_red > factor * hist_red.max())
    max_red = 255 - np.argmax(np.flip(hist_red) > factor * np.flip(hist_red).max())

    return min_blue, max_blue, min_green, max_green, min_red, max_red

def create_mapping_table(image, min_limits, max_limits):
    # 根据给定的上下限值构建映射表
    mapping_table = np.zeros((256,), dtype=np.uint8)

    for i in range(256):
        if i < min_limits:
            mapping_table[i] = 0
        elif i > max_limits:
            mapping_table[i] = 255
        else:
            mapping_table[i] = 255 * (i - min_limits) / (max_limits - min_limits)

    # 应用映射表
    mapped_image = cv2.LUT(image, mapping_table)

    return mapped_image

# 读取图像
input_image_path = "111/47.jpg"
image = cv2.imread(input_image_path)

# 设定参数
factor = 0.1

# 计算上下限值
min_blue, max_blue, min_green, max_green, min_red, max_red = calculate_limits(image, factor)

# 构建映射表并应用
mapped_image_blue = create_mapping_table(image[:, :, 0], min_blue, max_blue)
mapped_image_green = create_mapping_table(image[:, :, 1], min_green, max_green)
mapped_image_red = create_mapping_table(image[:, :, 2], min_red, max_red)

# 合并三个通道的映射结果
mapped_image = cv2.merge([mapped_image_blue, mapped_image_green, mapped_image_red])

# 显示原始图像和映射后的图像
cv2.imshow("Original Image", image)
cv2.imshow("Mapped Image", mapped_image)
RGB(image)
RGB(mapped_image)
cv2.imwrite("auto_color/47.jpg", mapped_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
