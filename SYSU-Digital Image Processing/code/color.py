import cv2
import numpy as np
from RGB import RGB

#直方图均衡化
def he(image):
    B,G,R = cv2.split(image)
    B = cv2.equalizeHist(B)
    G = cv2.equalizeHist(G)
    R = cv2.equalizeHist(R)
    result = cv2.merge((B,G,R))
    return result

#对比度限制的自适应均衡化直方图算法：
def clahe(image,clipLimit=2.0, tileGridSize=(8, 8)):
    B,G,R = cv2.split(image)
    clahe = cv2.createCLAHE(clipLimit, tileGridSize)
    clahe_B = clahe.apply(B)
    clahe_G = clahe.apply(G)
    clahe_R = clahe.apply(R)
    result = cv2.merge((clahe_B,clahe_G,clahe_R))
    cv2.imshow('clahe', result)
    return result


# 读取水下图像
underwater_img = cv2.imread('111/3.jpg')
restored_img = clahe(underwater_img)
# 显示结果
cv2.imshow('Underwater Image', underwater_img)
cv2.imshow('Restored Image', restored_img)
cv2.imwrite("clahe/3.jpg", restored_img)
RGB(underwater_img)
RGB(restored_img)
cv2.waitKey(0)
cv2.destroyAllWindows()